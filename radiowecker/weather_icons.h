const unsigned char weather_icons PROGMEM [12][1536] = {
	{//[0] '0_clear', 30x25px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x83, 0x04, 0x00, 0x00, 0x60, 0x18, 0x00, 0x00, 0x27, 0x90, 0x00, 
	0x00, 0x08, 0x40, 0x00, 0x00, 0x10, 0x20, 0x00, 0x00, 0x30, 0x30, 0x00, 0x03, 0xf0, 0x3f, 0x00, 
	0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x18, 0x60, 0x00, 0x00, 0x0f, 0xc0, 0x00, 
	0x00, 0x63, 0x18, 0x00, 0x00, 0xc3, 0x0c, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  },{ //[1] '45_48_fog', 30x25px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0x00, 
	0x07, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 
	0x07, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  },{//[2] '1_mostly-clear', 30x25px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 
	0x00, 0x40, 0x08, 0x00, 0x00, 0x27, 0x90, 0x00, 0x00, 0x0d, 0xc0, 0x00, 0x00, 0x10, 0x20, 0x00, 
	0x00, 0x30, 0x30, 0x00, 0x03, 0xb0, 0x17, 0x00, 0x03, 0xa0, 0x37, 0x00, 0x00, 0x30, 0x78, 0x00, 
	0x00, 0x10, 0x86, 0x00, 0x00, 0x0f, 0x01, 0x00, 0x00, 0x26, 0x00, 0x80, 0x00, 0x48, 0x00, 0x40, 
	0x00, 0x88, 0x00, 0x40, 0x00, 0x08, 0x00, 0x40, 0x00, 0x0c, 0x00, 0xc0, 0x00, 0x03, 0xff, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  },{//[3] '2_partly-cloudy', 30x25px
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x21, 0x08, 0x00, 0x00, 0x11, 0x10, 0x00, 0x00, 
	0x0f, 0xe7, 0xc0, 0x00, 0x0c, 0x6c, 0x30, 0x00, 0x08, 0x30, 0x1e, 0x00, 0xf8, 0x30, 0x0f, 0x00, 
	0x08, 0xe0, 0x01, 0x80, 0x0f, 0xc0, 0x00, 0xc0, 0x0e, 0x00, 0x00, 0x20, 0x1c, 0x00, 0x00, 0x10, 
	0x28, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x08, 0x00, 0x00, 0x10, 0x0c, 0x00, 0x00, 0x30, 
	0x04, 0x00, 0x00, 0x20, 0x03, 0x80, 0x01, 0xc0, 0x01, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  },{//[4] '3_overcast-cloudy', 30x25px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x00, 0x00, 0xc3, 0x00, 
	0x00, 0x0f, 0x80, 0x80, 0x00, 0x38, 0x40, 0x40, 0x00, 0x60, 0x38, 0x20, 0x00, 0x40, 0x1c, 0x20, 
	0x03, 0xc0, 0x02, 0x20, 0x07, 0x80, 0x01, 0xc0, 0x08, 0x00, 0x00, 0xc0, 0x10, 0x00, 0x00, 0x60, 
	0x10, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x60, 0x10, 0x00, 0x00, 0x40, 
	0x08, 0x00, 0x00, 0xc0, 0x0e, 0x00, 0x03, 0x80, 0x03, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  },{//[5] '51_53_55_80_drizzle', 30x25px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 
	0x00, 0x30, 0x60, 0x00, 0x00, 0x60, 0x3c, 0x00, 0x00, 0x40, 0x06, 0x00, 0x07, 0xc0, 0x03, 0x00, 
	0x0c, 0x00, 0x01, 0x80, 0x18, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x60, 
	0x30, 0x00, 0x00, 0x60, 0x10, 0x00, 0x00, 0x60, 0x10, 0x22, 0x00, 0x40, 0x08, 0x04, 0x00, 0xc0, 
	0x07, 0x80, 0x07, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 
	0x00, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  },{//[6] '56_57_freezingdrizzle', 30x25px
	0x00, 0x07, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x30, 0x70, 0x00, 0x00, 0x20, 0x38, 0x00, 
	0x01, 0xe0, 0x04, 0x00, 0x03, 0x00, 0x03, 0x00, 0x04, 0x00, 0x01, 0x80, 0x0c, 0x00, 0x01, 0x80, 
	0x0c, 0x00, 0x00, 0x80, 0x0c, 0x00, 0x00, 0x80, 0x04, 0x02, 0x01, 0x80, 0x02, 0x04, 0x03, 0x00, 
	0x03, 0xc8, 0x0e, 0x00, 0x00, 0x10, 0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x7a, 0x40, 0x00, 0x00, 0x7a, 0x20, 0x00, 
	0x00, 0x7b, 0x18, 0x00, 0x00, 0x37, 0x0c, 0x00, 0x00, 0x1c, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  },{//[7] '66_67_sleet', 30x25px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 
	0x00, 0x30, 0x60, 0x00, 0x00, 0x60, 0x3c, 0x00, 0x00, 0x40, 0x06, 0x00, 0x07, 0xc0, 0x03, 0x00, 
	0x0c, 0x00, 0x01, 0x80, 0x18, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x60, 
	0x30, 0x00, 0x00, 0x60, 0x10, 0x00, 0x00, 0x60, 0x10, 0x01, 0x00, 0x40, 0x08, 0x00, 0x40, 0xc0, 
	0x07, 0x80, 0x87, 0x00, 0x01, 0x18, 0x04, 0x00, 0x00, 0x3c, 0x90, 0x00, 0x00, 0x7c, 0x20, 0x00, 
	0x00, 0x3c, 0x44, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  },{//[8] '81_82_61_63_65_rain', 30x25px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 
	0x00, 0x30, 0x60, 0x00, 0x00, 0x60, 0x3c, 0x00, 0x00, 0x40, 0x06, 0x00, 0x07, 0xc0, 0x03, 0x00, 
	0x0c, 0x00, 0x01, 0x80, 0x18, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x60, 
	0x30, 0x00, 0x00, 0x60, 0x10, 0x00, 0x00, 0x60, 0x10, 0xcc, 0x80, 0x40, 0x08, 0x66, 0xc0, 0xc0, 
	0x07, 0xb6, 0x67, 0x00, 0x01, 0x13, 0x34, 0x00, 0x00, 0x19, 0xb0, 0x00, 0x00, 0x0c, 0x98, 0x00, 
	0x00, 0x06, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  },{//[9] '95_lightning', 30x25px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 
	0x00, 0x30, 0x60, 0x00, 0x00, 0x60, 0x3c, 0x00, 0x00, 0x40, 0x06, 0x00, 0x07, 0xc0, 0x03, 0x00, 
	0x0c, 0x00, 0x01, 0x80, 0x18, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x60, 0x30, 0x00, 0x00, 0x60, 
	0x30, 0x00, 0x00, 0x60, 0x10, 0x02, 0x00, 0x60, 0x10, 0x07, 0x00, 0x40, 0x08, 0x06, 0x00, 0xc0, 
	0x07, 0xee, 0x1f, 0x00, 0x01, 0xcf, 0x9c, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  },{//[10] '77_85_86_71_73_75_snow', 30x25px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 
	0x00, 0x30, 0x60, 0x00, 0x00, 0x60, 0x3c, 0x00, 0x00, 0x40, 0x06, 0x00, 0x07, 0xc0, 0x03, 0x00, 
	0x0c, 0x00, 0x01, 0x80, 0x18, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x60, 0x30, 0x00, 0x40, 0x60, 
	0x30, 0x00, 0xf0, 0x60, 0x10, 0x19, 0xf0, 0x60, 0x10, 0x3d, 0xf0, 0x40, 0x08, 0x7c, 0xf0, 0xc0, 
	0x07, 0xbc, 0x67, 0x00, 0x01, 0x3d, 0xc4, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xe0, 0x00, 
	0x00, 0x03, 0xc0, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  },{//[11] '96_99_thunderstorm', 30x25px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 
	0x00, 0x36, 0x60, 0x00, 0x00, 0x6e, 0x3c, 0x00, 0x00, 0x4e, 0x06, 0x00, 0x07, 0xcc, 0x03, 0x00, 
	0x0c, 0x0f, 0x81, 0x80, 0x18, 0x1f, 0x00, 0x40, 0x10, 0x03, 0x00, 0x60, 0x30, 0x02, 0x00, 0x60, 
	0x30, 0x06, 0x00, 0x60, 0x10, 0x00, 0x00, 0x60, 0x10, 0xcc, 0x80, 0x40, 0x08, 0x66, 0xc0, 0xc0, 
	0x07, 0xb6, 0x67, 0x00, 0x01, 0x13, 0x34, 0x00, 0x00, 0x19, 0xb0, 0x00, 0x00, 0x0c, 0x98, 0x00, 
	0x00, 0x06, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
  }
};