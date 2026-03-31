<?php
declare(strict_types=1);

/**
 * i18n.cpp: C-String-Arrays parsen und mit UTF-8-Inhalt zurückschreiben (Datei UTF-8 ohne BOM).
 */

function i18n_find_closing_brace(string $s, int $openIdx): int
{
    $i = $openIdx + 1;
    $n = strlen($s);
    $inStr = false;
    $esc = false;
    while ($i < $n) {
        $c = $s[$i++];
        if ($inStr) {
            if ($esc) {
                $esc = false;
            } elseif ($c === '\\') {
                $esc = true;
            } elseif ($c === '"') {
                $inStr = false;
            }
        } else {
            if ($c === '"') {
                $inStr = true;
            } elseif ($c === '}') {
                return $i - 1;
            }
        }
    }
    return -1;
}

function i18n_parse_one_string(string $body, int &$i): string
{
    $buf = '';
    $n = strlen($body);
    while ($i < $n) {
        $c = $body[$i++];
        if ($c === '\\') {
            if ($i >= $n) {
                break;
            }
            $c2 = $body[$i++];
            if ($c2 === 'n') {
                $buf .= "\n";
            } elseif ($c2 === 'r') {
                $buf .= "\r";
            } elseif ($c2 === 't') {
                $buf .= "\t";
            } elseif ($c2 === '\\') {
                $buf .= '\\';
            } elseif ($c2 === '"') {
                $buf .= '"';
            } elseif ($c2 === 'x' && $i + 1 < $n) {
                $hex = '';
                while ($i < $n && strlen($hex) < 2 && ctype_xdigit($body[$i])) {
                    $hex .= $body[$i++];
                }
                if (strlen($hex) === 2) {
                    $buf .= chr((int) hexdec($hex));
                }
            } else {
                $buf .= $c2;
            }
        } elseif ($c === '"') {
            return $buf;
        } else {
            $buf .= $c;
        }
    }
    return $buf;
}

function i18n_parse_literals(string $body): array
{
    $out = [];
    $i = 0;
    $n = strlen($body);
    while ($i < $n) {
        while ($i < $n && (ctype_space($body[$i]) || $body[$i] === ',')) {
            $i++;
        }
        if ($i >= $n) {
            break;
        }
        if ($body[$i] !== '"') {
            $i++;
            continue;
        }
        $i++;
        $out[] = i18n_parse_one_string($body, $i);
    }
    return $out;
}

/**
 * Findet den Block static const char *const NAME[…] = { (flexibel Whitespace).
 * Trifft absichtlich nicht auf Zeilen mit zwei *const (z. B. S_TAB-Zeigerarray).
 *
 * @return array{open:int,close:int}|null
 */
function i18n_locate_string_array(string $content, string $name): ?array
{
    if (!preg_match('/^[A-Z][A-Z0-9_]*$/', $name)) {
        return null;
    }
    $pattern = '/static\s+const\s+char\s*\*\s*const\s+' . preg_quote($name, '/') . '\s*\[[^\]]*\]\s*=\s*\{/';
    if (!preg_match($pattern, $content, $m, PREG_OFFSET_CAPTURE)) {
        return null;
    }
    $matched = $m[0][0];
    $openIdx = $m[0][1] + strlen($matched) - 1;
    if (($content[$openIdx] ?? '') !== '{') {
        return null;
    }
    $closeIdx = i18n_find_closing_brace($content, $openIdx);
    if ($closeIdx < 0) {
        return null;
    }

    return ['open' => $openIdx, 'close' => $closeIdx];
}

function i18n_extract_array_body(string $content, string $name): ?string
{
    $loc = i18n_locate_string_array($content, $name);
    if ($loc === null) {
        return null;
    }

    return substr($content, $loc['open'] + 1, $loc['close'] - $loc['open'] - 1);
}

function i18n_replace_array_body(string $content, string $name, array $strings): string
{
    $loc = i18n_locate_string_array($content, $name);
    if ($loc === null) {
        throw new RuntimeException(
            'Array-Kopf nicht gefunden: ' . $name . ' — bitte die aktuelle i18n.cpp aus dem Repository hochladen (mit S_ES/S_NL u. a.).'
        );
    }
    $openIdx = $loc['open'];
    $closeIdx = $loc['close'];
    $innerStart = $openIdx + 1;
    $formatted = i18n_format_array_inner($strings);

    return substr($content, 0, $innerStart) . "\n" . $formatted . "\n" . substr($content, $closeIdx);
}

/** Volles C-Array inkl. Kopf und schließender }; (für Einfügen fehlender Blöcke). */
function i18n_format_full_array(string $name, array $strings): string
{
    $head = i18n_array_decl_needle($name);

    return rtrim($head) . "\n" . i18n_format_array_inner($strings) . "\n};";
}

/**
 * Fehlende ES/NL-Arrays (ältere i18n.cpp) vor den jeweiligen *_TAB-Zeilen einfügen.
 */
function i18n_cpp_insert_missing_arrays(string $content, array $data): string
{
    $plans = [
        ['S_ES', 'S_NL', 'static const char *const *const S_TAB'],
        ['DSHORT_ES', 'DSHORT_NL', 'static const char *const *const DSHORT_TAB'],
        ['DLONG_ES', 'DLONG_NL', 'static const char *const *const DLONG_TAB'],
        ['MO_ES', 'MO_NL', 'static const char *const *const MO_TAB'],
    ];
    foreach ($plans as $step) {
        $a = $step[0];
        $b = $step[1];
        $marker = $step[2];
        foreach ([$a, $b] as $key) {
            if (!isset($data[$key]) || !is_array($data[$key])) {
                continue;
            }
            if (i18n_locate_string_array($content, $key) !== null) {
                continue;
            }
            $pos = strpos($content, $marker);
            if ($pos === false) {
                throw new RuntimeException(
                    'i18n.cpp kann nicht automatisch erweitert werden (Marker fehlt: ' . $marker . '). Bitte die aktuelle i18n.cpp aus dem Repository verwenden.'
                );
            }
            $block = i18n_format_full_array($key, $data[$key]) . "\n\n";
            $content = substr($content, 0, $pos) . $block . substr($content, $pos);
        }
    }

    return $content;
}

/** Zeigerzeilen S_TAB etc. von [4] auf [6] und um ES/NL ergänzen (nur wenn noch alt). */
function i18n_cpp_upgrade_tab_lines(string $content): string
{
    $replacements = [
        'static const char *const *const S_TAB[4] = {S_DE, S_EN, S_FR, S_RU};' =>
            'static const char *const *const S_TAB[6] = {S_DE, S_EN, S_FR, S_RU, S_ES, S_NL};',
        'static const char *const *const DSHORT_TAB[4] = {DSHORT_DE, DSHORT_EN, DSHORT_FR, DSHORT_RU};' =>
            'static const char *const *const DSHORT_TAB[6] = {DSHORT_DE, DSHORT_EN, DSHORT_FR, DSHORT_RU, DSHORT_ES, DSHORT_NL};',
        'static const char *const *const DLONG_TAB[4] = {DLONG_DE, DLONG_EN, DLONG_FR, DLONG_RU};' =>
            'static const char *const *const DLONG_TAB[6] = {DLONG_DE, DLONG_EN, DLONG_FR, DLONG_RU, DLONG_ES, DLONG_NL};',
        'static const char *const *const MO_TAB[4] = {MO_DE, MO_EN, MO_FR, MO_RU};' =>
            'static const char *const *const MO_TAB[6] = {MO_DE, MO_EN, MO_FR, MO_RU, MO_ES, MO_NL};',
        'static const char *const LANG_CODES[4] = {"de", "en", "fr", "ru"};' =>
            'static const char *const LANG_CODES[6] = {"de", "en", "fr", "ru", "es", "nl"};',
    ];
    foreach ($replacements as $from => $to) {
        if (strpos($content, $from) !== false) {
            $content = str_replace($from, $to, $content);
        }
    }

    return $content;
}

function i18n_c_escape(string $s): string
{
    $s = str_replace('\\', '\\\\', $s);
    $s = str_replace('"', '\\"', $s);
    $s = str_replace("\r\n", "\n", $s);
    $s = str_replace("\r", '\r', $s);
    $s = str_replace("\n", '\\n', $s);
    $s = str_replace("\t", '\\t', $s);
    return $s;
}

function i18n_format_array_inner(array $strings): string
{
    $lines = [];
    foreach ($strings as $s) {
        $lines[] = '    "' . i18n_c_escape($s) . '"';
    }
    return implode(",\n", $lines);
}

/** C-Deklaration wie im Sketch: S_* ohne Größe, DSHORT/DLONG [7], MO [12], LANG_CODES [6] */
function i18n_array_decl_needle(string $name): string
{
    static $bracket = [
        'DSHORT_DE' => '[7]', 'DSHORT_EN' => '[7]', 'DSHORT_FR' => '[7]', 'DSHORT_RU' => '[7]', 'DSHORT_ES' => '[7]', 'DSHORT_NL' => '[7]',
        'DLONG_DE' => '[7]', 'DLONG_EN' => '[7]', 'DLONG_FR' => '[7]', 'DLONG_RU' => '[7]', 'DLONG_ES' => '[7]', 'DLONG_NL' => '[7]',
        'MO_DE' => '[12]', 'MO_EN' => '[12]', 'MO_FR' => '[12]', 'MO_RU' => '[12]', 'MO_ES' => '[12]', 'MO_NL' => '[12]',
        'LANG_CODES' => '[6]',
    ];
    $dim = $bracket[$name] ?? '[]';

    return 'static const char *const ' . $name . $dim . ' = {';
}

/** Fehlende ES/NL-Blöcke in $out mit EN füllen (nach i18n_extract, vor UI). */
function i18n_parse_cpp_apply_defaults(array &$out): void
{
    if (($out['S_DE'] ?? null) === null) {
        throw new RuntimeException(
            'Block fehlt: S_DE — keine gültige Radiowecker-i18n.cpp (oder Datei beschädigt).'
        );
    }
    $nMain = count($out['S_DE']);
    $fillMain = static function (string $key, ?array $from) use (&$out, $nMain): void {
        if (($out[$key] ?? null) !== null) {
            return;
        }
        if ($from !== null && count($from) === $nMain) {
            $out[$key] = $from;

            return;
        }
        $out[$key] = array_fill(0, $nMain, '');
    };
    $fillMain('S_ES', $out['S_EN'] ?? null);
    $fillMain('S_NL', $out['S_EN'] ?? null);

    $fill7 = static function (string $key, ?array $from) use (&$out): void {
        if (($out[$key] ?? null) !== null) {
            return;
        }
        if ($from !== null && count($from) === 7) {
            $out[$key] = $from;

            return;
        }
        $out[$key] = array_fill(0, 7, '');
    };
    $fill7('DSHORT_ES', $out['DSHORT_EN'] ?? null);
    $fill7('DSHORT_NL', $out['DSHORT_EN'] ?? null);
    $fill7('DLONG_ES', $out['DLONG_EN'] ?? null);
    $fill7('DLONG_NL', $out['DLONG_EN'] ?? null);
    $fill7('MO_ES', $out['MO_EN'] ?? null);
    $fill7('MO_NL', $out['MO_EN'] ?? null);

    if (($out['LANG_CODES'] ?? null) === null) {
        $out['LANG_CODES'] = ['de', 'en', 'fr', 'ru', 'es', 'nl'];
    } else {
        $lc = $out['LANG_CODES'];
        $n = count($lc);
        if ($n === 4) {
            $out['LANG_CODES'] = array_merge($lc, ['es', 'nl']);
        } elseif ($n !== 6) {
            $pad = ['de', 'en', 'fr', 'ru', 'es', 'nl'];
            $out['LANG_CODES'] = [];
            for ($i = 0; $i < 6; $i++) {
                $out['LANG_CODES'][$i] = $lc[$i] ?? $pad[$i];
            }
        }
    }

    $required = [
        'S_EN', 'S_FR', 'S_RU',
        'DSHORT_DE', 'DSHORT_EN', 'DSHORT_FR', 'DSHORT_RU',
        'DLONG_DE', 'DLONG_EN', 'DLONG_FR', 'DLONG_RU',
        'MO_DE', 'MO_EN', 'MO_FR', 'MO_RU',
    ];
    foreach ($required as $req) {
        if (($out[$req] ?? null) === null) {
            throw new RuntimeException('Block fehlt: ' . $req);
        }
    }
}

/** @return array<string, array<int, string>> */
function i18n_parse_cpp(string $content): array
{
    $names = [
        'S_DE', 'S_EN', 'S_FR', 'S_RU', 'S_ES', 'S_NL',
        'DSHORT_DE', 'DSHORT_EN', 'DSHORT_FR', 'DSHORT_RU', 'DSHORT_ES', 'DSHORT_NL',
        'DLONG_DE', 'DLONG_EN', 'DLONG_FR', 'DLONG_RU', 'DLONG_ES', 'DLONG_NL',
        'MO_DE', 'MO_EN', 'MO_FR', 'MO_RU', 'MO_ES', 'MO_NL',
        'LANG_CODES',
    ];
    $out = [];
    foreach ($names as $name) {
        $body = i18n_extract_array_body($content, $name);
        if ($body === null) {
            $out[$name] = null;
        } else {
            $out[$name] = i18n_parse_literals($body);
        }
    }
    i18n_parse_cpp_apply_defaults($out);

    return $out;
}

function i18n_write_cpp(string $content, array $data): string
{
    $content = i18n_cpp_insert_missing_arrays($content, $data);
    $content = i18n_cpp_upgrade_tab_lines($content);
    foreach ($data as $key => $strings) {
        if (!is_array($strings)) {
            continue;
        }
        $content = i18n_replace_array_body($content, (string) $key, $strings);
    }

    return $content;
}

/** Enum-Schlüssel für UI-Strings (ohne I18N_STR_COUNT); Zeilen mit „= 0“ etc. */
function i18n_enum_keys(string $hContent): array
{
    if (!preg_match_all('/^\s*(I18N_[A-Z0-9_]+)(?:\s*=\s*[^,]+)?\s*,?\s*$/m', $hContent, $m)) {
        return [];
    }
    return array_values(array_filter($m[1], static fn (string $k): bool => $k !== 'I18N_STR_COUNT'));
}
