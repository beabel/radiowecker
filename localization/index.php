<?php
declare(strict_types=1);

require __DIR__ . '/i18n_lib.php';
$config = require __DIR__ . '/config.php';

if (session_status() !== PHP_SESSION_ACTIVE) {
    session_start();
}

function h(string $s): string
{
    return htmlspecialchars($s, ENT_QUOTES | ENT_SUBSTITUTE, 'UTF-8');
}

function i18n_strip_bom(string $s): string
{
    if (strncmp($s, "\xEF\xBB\xBF", 3) === 0) {
        return substr($s, 3);
    }
    return $s;
}

$err = '';
$ok = '';
$maxB = (int) $config['max_upload_bytes'];

// ——— Download (vor HTML-Output)
if (isset($_GET['download'])) {
    $kind = $_GET['download'];
    if ($kind === 'cpp' && !empty($_SESSION['i18n_cpp'])) {
        if (empty($_SESSION['i18n_cpp_saved'])) {
            header('HTTP/1.1 403 Forbidden');
            header('Content-Type: text/plain; charset=utf-8');
            echo 'i18n.cpp steht erst nach „Änderungen übernehmen“ zum Download bereit (sonst wäre es nur die unveränderte Upload-Datei).';
            exit;
        }
        header('Content-Type: text/plain; charset=utf-8');
        header('Content-Disposition: attachment; filename="i18n.cpp"');
        echo $_SESSION['i18n_cpp'];
        exit;
    }
    header('HTTP/1.1 404 Not Found');
    echo 'Unbekannter Download oder nichts in der Sitzung.';
    exit;
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $action = $_POST['action'] ?? '';

    if ($action === 'clear') {
        unset($_SESSION['i18n_cpp'], $_SESSION['i18n_h'], $_SESSION['i18n_cpp_saved']);
        header('Location: index.php');
        exit;
    }

    if ($action === 'upload') {
        try {
            if (!isset($_FILES['file_cpp']) || !is_array($_FILES['file_cpp'])) {
                throw new RuntimeException('Bitte i18n.cpp auswählen.');
            }
            $fe = (int) ($_FILES['file_cpp']['error'] ?? UPLOAD_ERR_NO_FILE);
            if ($fe !== UPLOAD_ERR_OK) {
                throw new RuntimeException('Upload i18n.cpp fehlgeschlagen (Code ' . $fe . ').');
            }
            if (($_FILES['file_cpp']['size'] ?? 0) > $maxB) {
                throw new RuntimeException('i18n.cpp zu groß (max. ' . round($maxB / 1024) . ' KB).');
            }
            $cpp = file_get_contents($_FILES['file_cpp']['tmp_name']);
            if ($cpp === false) {
                throw new RuntimeException('i18n.cpp konnte nicht gelesen werden.');
            }
            $cpp = i18n_strip_bom($cpp);
            i18n_parse_cpp($cpp);

            $h = '';
            if (isset($_FILES['file_h']) && is_array($_FILES['file_h']) && (int) $_FILES['file_h']['error'] === UPLOAD_ERR_OK) {
                if (($_FILES['file_h']['size'] ?? 0) > $maxB) {
                    throw new RuntimeException('i18n.h zu groß.');
                }
                $hRaw = file_get_contents($_FILES['file_h']['tmp_name']);
                if ($hRaw !== false) {
                    $h = i18n_strip_bom($hRaw);
                }
            }

            $_SESSION['i18n_cpp'] = $cpp;
            $_SESSION['i18n_h'] = $h;
            $_SESSION['i18n_cpp_saved'] = false;
            header('Location: index.php?uploaded=1');
            exit;
        } catch (Throwable $e) {
            $err = $e->getMessage();
        }
    }

    if ($action === 'save') {
        try {
            $content = $_SESSION['i18n_cpp'] ?? '';
            if ($content === '') {
                throw new RuntimeException('Keine i18n.cpp in der Sitzung. Bitte erneut hochladen.');
            }
            $parsed = i18n_parse_cpp($content);

            $merge = static function (string $key, int $expected) use (&$parsed): void {
                if (!isset($_POST[$key]) || !is_array($_POST[$key])) {
                    throw new RuntimeException('Feldgruppe fehlt: ' . $key);
                }
                $arr = $_POST[$key];
                if (count($arr) !== $expected) {
                    throw new RuntimeException($key . ': erwartet ' . $expected . ' Einträge, erhalten ' . count($arr));
                }
                $parsed[$key] = array_map(static function ($v): string {
                    if (!is_string($v)) {
                        return '';
                    }
                    return str_replace("\r\n", "\n", $v);
                }, $arr);
            };

            $hContent = (string) ($_SESSION['i18n_h'] ?? '');
            $enumKeys = i18n_enum_keys($hContent);
            $nMain = count($enumKeys);
            if ($nMain === 0) {
                $nMain = count($parsed['S_DE'] ?? []);
            }
            if ($nMain === 0) {
                throw new RuntimeException('Keine UI-Strings erkannt (i18n.h mit Enum hochladen oder gültige i18n.cpp).');
            }

            $merge('S_DE', $nMain);
            $merge('S_EN', $nMain);
            $merge('S_FR', $nMain);
            $merge('S_RU', $nMain);
            $merge('S_ES', $nMain);
            $merge('S_NL', $nMain);
            $merge('DSHORT_DE', 7);
            $merge('DSHORT_EN', 7);
            $merge('DSHORT_FR', 7);
            $merge('DSHORT_RU', 7);
            $merge('DSHORT_ES', 7);
            $merge('DSHORT_NL', 7);
            $merge('DLONG_DE', 7);
            $merge('DLONG_EN', 7);
            $merge('DLONG_FR', 7);
            $merge('DLONG_RU', 7);
            $merge('DLONG_ES', 7);
            $merge('DLONG_NL', 7);
            $merge('MO_DE', 12);
            $merge('MO_EN', 12);
            $merge('MO_FR', 12);
            $merge('MO_RU', 12);
            $merge('MO_ES', 12);
            $merge('MO_NL', 12);
            $merge('LANG_CODES', 6);

            $_SESSION['i18n_cpp'] = i18n_write_cpp($content, $parsed);
            $_SESSION['i18n_cpp_saved'] = true;
            header('Location: index.php?saved=1');
            exit;
        } catch (Throwable $e) {
            $err = $e->getMessage();
        }
    }
}

if (isset($_GET['uploaded'])) {
    $ok = 'Dateien übernommen. Texte bearbeiten und „Änderungen übernehmen“ — danach steht der Download von i18n.cpp bereit.';
}
if (isset($_GET['saved'])) {
    $ok = 'Änderungen in der Sitzung gespeichert. Jetzt i18n.cpp herunterladen und in den Sketch-Ordner legen.';
}

header('Content-Type: text/html; charset=utf-8');

$hasCpp = !empty($_SESSION['i18n_cpp']);
$hasH = !empty($_SESSION['i18n_h']);
$canDownloadCpp = !empty($_SESSION['i18n_cpp_saved']);
$data = [];
$enumKeys = [];

if ($hasCpp) {
    try {
        $data = i18n_parse_cpp((string) $_SESSION['i18n_cpp']);
    } catch (Throwable $e) {
        $err = $err ? $err . ' ' : '';
        $err .= 'Parse: ' . $e->getMessage();
    }
}

if ($hasH) {
    $enumKeys = i18n_enum_keys((string) $_SESSION['i18n_h']);
}
if ($enumKeys === [] && isset($data['S_DE'])) {
    $enumKeys = array_map(static fn (int $i): string => 'I18N_' . $i, range(0, count($data['S_DE']) - 1));
}

$wdayTitles = ['0 Sonntag', '1 Montag', '2 Dienstag', '3 Mittwoch', '4 Donnerstag', '5 Freitag', '6 Samstag'];
$monthTitles = ['Jan', 'Feb', 'Mär', 'Apr', 'Mai', 'Jun', 'Jul', 'Aug', 'Sep', 'Okt', 'Nov', 'Dez'];
$langCodeLabels = ['de', 'en', 'fr', 'ru', 'es', 'nl'];

$cyrAll = 'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя';
/* Entspricht radiowecker/lv_font_latin_supp_14.c + lv_font_cyrl_supp_14 (Kyrillisch separat). */
$latinTftChars = 'áéíóúñÑ¿¡àâäéèêëïîôùûüçœæÀÂÄÆÇÈÉÊËÎÏÑÔÖÙÛÜŒœß«»…';
$deChars = 'äöüßÄÖÜ';
/* „ “ ” (U+201C/U+201D/U+201E): nicht im 1-bpp-Latin-Supplement — nur Web/Zwischenablage. */
$punctWebOnly = '„“”';
$cyrButtons = preg_split('//u', $cyrAll, -1, PREG_SPLIT_NO_EMPTY) ?: [];
$latinTftButtons = array_values(array_unique(preg_split('//u', $latinTftChars . $deChars, -1, PREG_SPLIT_NO_EMPTY) ?: []));
$webPunctButtons = preg_split('//u', $punctWebOnly, -1, PREG_SPLIT_NO_EMPTY) ?: [];

?>
<!DOCTYPE html>
<html lang="de">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Radiowecker – UI-Texte (i18n)</title>
  <style>
    :root { font-family: system-ui, "Segoe UI", Roboto, sans-serif; }
    body { max-width: 1200px; margin: 1rem auto; padding: 0 1rem; background: #f4f4f5; }
    h1 { font-size: 1.35rem; }
    h2 { font-size: 1.1rem; margin-top: 2rem; border-bottom: 1px solid #ccc; padding-bottom: .25rem; }
    .msg-ok { background: #d1fae5; color: #065f46; padding: .75rem; border-radius: 6px; margin: 1rem 0; }
    .msg-err { background: #fee2e2; color: #991b1b; padding: .75rem; border-radius: 6px; margin: 1rem 0; }
    .hint { color: #52525b; font-size: .9rem; margin: .5rem 0 1rem; }
    .upload-box { background: #fff; padding: 1.25rem; border-radius: 8px; box-shadow: 0 1px 3px #0001; margin-bottom: 1.5rem; }
    .upload-box label { display: block; margin: .5rem 0 .15rem; font-weight: 600; }
    .upload-box input[type=file] { margin-bottom: .75rem; }
    table.edit { width: 100%; border-collapse: collapse; background: #fff; border-radius: 8px; overflow: hidden; box-shadow: 0 1px 3px #0001; }
    table.edit th, table.edit td { border: 1px solid #e4e4e7; padding: .4rem .5rem; vertical-align: top; }
    table.edit th { background: #fafafa; text-align: left; font-weight: 600; }
    textarea.i18n { width: 100%; min-height: 2.5em; box-sizing: border-box; font-family: inherit; font-size: .9rem; }
    .key { font-family: ui-monospace, monospace; font-size: .78rem; color: #71717a; white-space: nowrap; }
    .palette { display: flex; flex-wrap: wrap; gap: .25rem; margin: .5rem 0 1rem; align-items: center; }
    .palette button { cursor: pointer; padding: .2rem .35rem; font-size: .85rem; border: 1px solid #d4d4d8; background: #fff; border-radius: 4px; }
    .palette button:hover { background: #e4e4e7; }
    .palette span { font-size: .75rem; color: #71717a; margin-right: .35rem; }
    .actions { margin: 1.5rem 0; display: flex; flex-wrap: wrap; gap: .75rem; align-items: center; }
    .actions button, .actions a.btn { padding: .55rem 1rem; font-size: 1rem; border-radius: 6px; cursor: pointer; text-decoration: none; display: inline-block; border: none; }
    .btn-save { background: #2563eb; color: #fff; }
    .btn-save:hover { background: #1d4ed8; }
    .btn-dl { background: #059669; color: #fff; }
    .btn-dl:hover { background: #047857; }
    .btn-clear { background: #fff; color: #444; border: 1px solid #d4d4d8 !important; }
    .btn-clear:hover { background: #f4f4f5; }
    code { background: #e4e4e7; padding: .1rem .3rem; border-radius: 3px; font-size: .85em; }
  </style>
</head>
<body>
  <h1>Radiowecker – UI-Texte bearbeiten</h1>

  <?php if ($ok): ?><div class="msg-ok"><?= h($ok) ?></div><?php endif; ?>
  <?php if ($err): ?><div class="msg-err"><?= h($err) ?></div><?php endif; ?>

  <?php if (!$hasCpp): ?>
  <div class="upload-box">
    <p class="hint"><strong>Schritt 1:</strong> Eigene Dateien aus dem Sketch-Ordner hochladen. Ohne <code>i18n.h</code> erscheinen die Enum-Namen als <code>I18N_0</code> … (bearbeiten geht trotzdem). Die Web-UI im Browser nutzt zusätzlich <code>index.h</code> (<code>I18N</code>-Objekt) — das wird hier nicht bearbeitet.</p>
    <form method="post" enctype="multipart/form-data" accept-charset="UTF-8">
      <input type="hidden" name="action" value="upload">
      <label for="file_cpp">i18n.cpp (Pflicht)</label>
      <input type="file" id="file_cpp" name="file_cpp" accept=".cpp,text/plain" required>
      <label for="file_h">i18n.h (optional, für Schlüsselnamen in der Tabelle)</label>
      <input type="file" id="file_h" name="file_h" accept=".h,.hpp,text/plain">
      <p class="hint">Max. <?= h((string) round($maxB / 1024)) ?> KB pro Datei. Alles nur in der PHP-Sitzung (Browser), nichts wird automatisch in dein Projekt geschrieben.</p>
      <button type="submit" class="btn-save" style="border:none;">Hochladen &amp; einlesen</button>
    </form>
  </div>
  <?php else: ?>

  <p class="hint">Texte als UTF-8 in den C-Strings; Zeilenumbruch in den Feldern wird zu <code>\n</code>. Dateien liegen nur in dieser Browser-Sitzung – nach „Änderungen übernehmen“ erscheint der Download für <code>i18n.cpp</code>; manuell in <code>radiowecker/</code> ersetzen. Zeichen unter „Latein (TFT)“ und „…«»“ sind in <code>lv_font_latin_supp_14.c</code> hinterlegt; „nur Web“ fehlen dort (TFT zeigt ggf. Platzhalter). <strong>Web-Oberfläche (Browser):</strong> weitere Übersetzungen stehen im Sketch in <code>radiowecker/index.h</code> im JavaScript-Objekt <code>I18N</code> (hier kein Editor). Neue <code>I18N_*</code>-Einträge in <code>i18n.h</code> erfordern dieselbe Anzahl neuer Strings in allen <code>S_*</code>-Arrays in <code>i18n.cpp</code>; die Tabellen auf dieser Seite folgen dann automatisch dem Enum.</p>

  <div class="actions" style="margin-top:0;">
    <?php if ($canDownloadCpp): ?>
    <a class="btn btn-dl" href="index.php?download=cpp">i18n.cpp herunterladen</a>
    <?php endif; ?>
    <form method="post" style="display:inline;" onsubmit="return confirm('Sitzung leeren und neue Dateien hochladen?');">
      <input type="hidden" name="action" value="clear">
      <button type="submit" class="btn-clear">Neue Dateien hochladen</button>
    </form>
  </div>

  <div class="palette" id="palette">
    <span>Klick = Einfügen:</span>
    <span>Kyrill.:</span>
    <?php foreach ($cyrButtons as $ch): ?>
    <button type="button" class="ins" data-ch="<?= h($ch) ?>"><?= h($ch) ?></button>
    <?php endforeach; ?>
    <span>Latein (TFT):</span>
    <?php foreach ($latinTftButtons as $ch): ?>
    <button type="button" class="ins" data-ch="<?= h($ch) ?>"><?= h($ch) ?></button>
    <?php endforeach; ?>
    <span>nur Web:</span>
    <?php foreach ($webPunctButtons as $ch): ?>
    <button type="button" class="ins" data-ch="<?= h($ch) ?>"><?= h($ch) ?></button>
    <?php endforeach; ?>
  </div>

  <form method="post" accept-charset="UTF-8" id="mainform">
    <input type="hidden" name="action" value="save">
    <h2>Menü- und Meldungstexte</h2>
    <table class="edit">
      <thead>
        <tr>
          <th>Schlüssel (i18n.h)</th>
          <th>Deutsch</th>
          <th>English</th>
          <th>Français</th>
          <th>Русский</th>
          <th>Español</th>
          <th>Nederlands</th>
        </tr>
      </thead>
      <tbody>
        <?php
        $n = count($enumKeys);
        for ($i = 0; $i < $n; $i++):
            $key = $enumKeys[$i] ?? ('#' . $i);
        ?>
        <tr>
          <td class="key"><?= h($key) ?></td>
          <?php foreach (['S_DE', 'S_EN', 'S_FR', 'S_RU', 'S_ES', 'S_NL'] as $an): ?>
          <td>
            <textarea class="i18n" name="<?= h($an) ?>[]"><?= h($data[$an][$i] ?? '') ?></textarea>
          </td>
          <?php endforeach; ?>
        </tr>
        <?php endfor; ?>
      </tbody>
    </table>

    <h2>Wochentage kurz (So … Sa, tm_wday 0–6)</h2>
    <table class="edit">
      <thead>
        <tr><th>Index</th><th>DE</th><th>EN</th><th>FR</th><th>RU</th><th>ES</th><th>NL</th></tr>
      </thead>
      <tbody>
        <?php for ($i = 0; $i < 7; $i++): ?>
        <tr>
          <td class="key"><?= h($wdayTitles[$i]) ?></td>
          <?php foreach (['DSHORT_DE', 'DSHORT_EN', 'DSHORT_FR', 'DSHORT_RU', 'DSHORT_ES', 'DSHORT_NL'] as $an): ?>
          <td><textarea class="i18n" name="<?= h($an) ?>[]"><?= h($data[$an][$i] ?? '') ?></textarea></td>
          <?php endforeach; ?>
        </tr>
        <?php endfor; ?>
      </tbody>
    </table>

    <h2>Wochentage lang</h2>
    <table class="edit">
      <thead>
        <tr><th>Index</th><th>DE</th><th>EN</th><th>FR</th><th>RU</th><th>ES</th><th>NL</th></tr>
      </thead>
      <tbody>
        <?php for ($i = 0; $i < 7; $i++): ?>
        <tr>
          <td class="key"><?= h($wdayTitles[$i]) ?></td>
          <?php foreach (['DLONG_DE', 'DLONG_EN', 'DLONG_FR', 'DLONG_RU', 'DLONG_ES', 'DLONG_NL'] as $an): ?>
          <td><textarea class="i18n" name="<?= h($an) ?>[]"><?= h($data[$an][$i] ?? '') ?></textarea></td>
          <?php endforeach; ?>
        </tr>
        <?php endfor; ?>
      </tbody>
    </table>

    <h2>Monate (kurz, tm_mon 0–11)</h2>
    <table class="edit">
      <thead>
        <tr><th>Monat</th><th>DE</th><th>EN</th><th>FR</th><th>RU</th><th>ES</th><th>NL</th></tr>
      </thead>
      <tbody>
        <?php for ($i = 0; $i < 12; $i++): ?>
        <tr>
          <td class="key"><?= h($monthTitles[$i]) ?></td>
          <?php foreach (['MO_DE', 'MO_EN', 'MO_FR', 'MO_RU', 'MO_ES', 'MO_NL'] as $an): ?>
          <td><textarea class="i18n" name="<?= h($an) ?>[]"><?= h($data[$an][$i] ?? '') ?></textarea></td>
          <?php endforeach; ?>
        </tr>
        <?php endfor; ?>
      </tbody>
    </table>

    <h2>Sprachcodes (Web/JSON)</h2>
    <table class="edit">
      <thead>
        <tr><th>Index</th><th>Code (z. B. de)</th></tr>
      </thead>
      <tbody>
        <?php for ($i = 0; $i < 6; $i++): ?>
        <tr>
          <td class="key"><?= h($langCodeLabels[$i]) ?></td>
          <td><textarea class="i18n" name="LANG_CODES[]" rows="1"><?= h($data['LANG_CODES'][$i] ?? '') ?></textarea></td>
        </tr>
        <?php endfor; ?>
      </tbody>
    </table>

    <div class="actions">
      <button type="submit" class="btn-save">Änderungen übernehmen (in Sitzung)</button>
      <?php if ($canDownloadCpp): ?>
      <a class="btn btn-dl" href="index.php?download=cpp">i18n.cpp herunterladen</a>
      <?php endif; ?>
    </div>
  </form>

  <script>
  (function() {
    var ta = null;
    document.querySelectorAll('textarea.i18n').forEach(function(el) {
      el.addEventListener('focus', function() { ta = el; });
    });
    document.querySelectorAll('button.ins').forEach(function(btn) {
      btn.addEventListener('click', function() {
        var ch = btn.getAttribute('data-ch');
        if (!ch || !ta) return;
        var s = ta.selectionStart, e = ta.selectionEnd;
        var v = ta.value;
        ta.value = v.slice(0, s) + ch + v.slice(e);
        var np = s + ch.length;
        ta.focus();
        ta.setSelectionRange(np, np);
      });
    });
  })();
  </script>

  <?php endif; ?>
</body>
</html>
