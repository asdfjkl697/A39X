/*************************************************************************************************
 * * 250312
 *  //* 揉肩 揉颈1 揉颈+凤池 揉凤池1 
	{ {{PX00,   PY02,    PZ00,   2,   RNZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX00,   PY01,    PZ01,   2,   RNF  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX00,   PY00,    PZ02,   2,   RNZFS}, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX00,   PY00,    PZ01,   2,   RNZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},

    //* 3D揉肩 揉颈3
    { {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2,  2},  0xACAC,   0xAAAA}, //* 循环次数X 条数X
	{ {{PX00,   PY02,    PZ01,   2,   D3RNZ}, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX00,   PY01,    PZ00,   2,   RNZFS}, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61}, //todo 修改 250311

    //* 揉肩1 3D揉拍肩 揉颈1 拍颈
	{ {{PX00,   PY02,    PZ00,   2,   RNZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX00,   PY02,    PZ01,   2,   D3RPZ}, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX00,   PY01,    PZ01,   2,   RNZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX00,   PY01,    PZ01,   2,   PD   }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},

    //* 向上推拿颈部
    { {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  3,  2},  0xACAC,   0xAAAA}, //* 循环次数X 条数X
	{ {{PX00,   PY03,    PZ00,   5,   ZY   }, 3     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},
	{ {{PX00,   PY00,    PZ02,   2,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},

    //* 向上提颈
    { {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  3,  3},  0xACAC,   0xAAAA}, //* 循环次数X 条数X  次数增多
	{ {{PX04,   PY01,    PZ02,   3,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX00,   PY01,    PZ02,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX00,   PY00,    PZ02,   1,   ZY   }, 20    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
  
	//* 拍打颈部
	{ {{PX00,   PY02,    PZ00,   2,   RNZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX00,   PY01,    PZ00,   2,   PD   }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},

	//* 移动拍打肩部
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2,  2},  0xACAC,   0xAAAA}, //* 循环次数X 条数X
	{ {{PX00,   PY02,    PZ00,   2,   PD   }, 50    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX02,   PY03,    PZ00,   2,   PD   }, 50    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	//* 节奏拍打肩部 
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2,  2},  0xACAC,   0xAAAA}, //* 循环次数X 条数X
	{ {{PX00,   PY02,    PZ00,   2,   PDB  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX02,   PY03,    PZ00,   2,   PD   }, 50    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },

	//* 开背向下
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2, 6},  0xACAC,   0xAAAA}, //* 循环次数X 条数X 	
	{ {{PX00,   PY03,    PZ00,   4,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX04,   PY03,    PZ01,   2,   ZY   }, 20    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY04,    PZ00,   4,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX04,   PY04,    PZ01,   2,   ZY   }, 20    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY05,    PZ00,   4,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX04,   PY05,    PZ01,   2,   ZY   }, 20    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },

	//* 压肩
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2,  3},  0xACAC,   0xAAAA}, //* 循环次数X 条数X
	{ {{PX03,   PY00,    PZ00,   4,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},
	{ {{PX03,   PY00,    PZ02,   4,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},
	{ {{PX03,   PY02,    PZ02,   1,   ZY   }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32}, 

	//* 揉拍肩下
	{ {{PX00,   PY02,    PZ00,   3,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},
	{ {{PX00,   PY03,    PZ01,   2,   RPZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},

	//* 揉捏3 颈部、凤池  备用
	{ {{PX00,   PY01,    PZ01,   2,   RNZB }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01}, 
	{ {{PX00,   PY01,    PZ01,   2,   RNFB }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01}, 
	{ {{PX00,   PY00,    PZ02,   2,   RNZB }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01}, 
	{ {{PX00,   PY00,    PZ02,   2,   RNFB }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01}, 	

    //* 揉捏走位下
	{ {{PX00,   PY02,    PZ02,   3,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},
	{ {{PX00,   PY04,    PZ00,   2,   RNZ  }, 10    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY06,    PZ00,   4,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY09,    PZ00,   2,   RNZ  }, 10    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	//* 揉捏走位下
	{ {{PX00,   PY08,    PZ00,   2,   RNZ  }, 5     ,600}, FA_GAS_FULLGAS3,     FA_MoveDw_AUTO01},
	//* 指压走位上
	{ {{PX00,   PY02,    PZ01,   4,   ZYS  }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
    //* 拍打走位下
	{ {{PX00,   PY08,    PZ01,   3,   PD   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
    //* 3D拍打走位上
    { {{PX00,   PY02,    PZ02,   2,   D3PD }, 10    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},
	//* 游动走位 向下
	{ {{PX00,   PY07,    PZ00,   2,   RNZFS}, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},

	//* 推拿脊椎来回
	{ {{PX00,   PY03,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},
	{ {{PX00,   PY03,    PZ01,   5,   TNDW }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},
	{ {{PX00,   PY04,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},
	{ {{PX00,   PY04,    PZ01,   5,   TNDW }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},
	{ {{PX00,   PY05,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO32},
	{ {{PX00,   PY05,    PZ02,   5,   TNDW }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY06,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY06,    PZ03,   5,   TNDW }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY07,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY07,    PZ03,   5,   TNDW }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY08,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY08,    PZ03,   5,   TNUP }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY07,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY07,    PZ03,   5,   TNUP }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY06,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY06,    PZ03,   5,   TNUP }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY05,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY05,    PZ02,   5,   TNUP }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY04,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY04,    PZ01,   5,   TNUP }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY03,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY03,    PZ01,   5,   TNUP }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY02,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},

   	//* 推拿腰部
	{ {{PX02,   PY07,    PZ00,   2,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2, 4},  0xACAC,   0xAAAA}, //* 循环次数X 条数X 	
	{ {{PX02,   PY07,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX02,   PY07,    PZ03,   5,   TNDW }, 40    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX02,   PY08,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX02,   PY08,    PZ03,   5,   TNUP }, 40    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	
	//* 指压腰部
	{ {{PX03,   PY07,    PZ00,   2,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2, 6},  0xACAC,   0xAAAA}, //* 循环次数X 条数X 	
	{ {{PX03,   PY07,    PZ02,   2,   ZYS  }, 50    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY07,    PZ02,   2,   ZYS  }, 50    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX03,   PY08,    PZ02,   2,   ZYS  }, 50    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY08,    PZ02,   2,   ZYS  }, 50    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX03,   PY09,    PZ02,   2,   ZYS  }, 50    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY09,    PZ02,   2,   ZYS  }, 50    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},

	//* 揉拍腰部
	{ {{PX00,   PY08,    PZ02,   3,   RPZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01}, 
	{ {{PX00,   PY09,    PZ02,   3,   RPZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	
	{ {{PX02,   PY02,    PZ02,   2,   ZYS  }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },

	{ {{PX00,   PY02,    PZ00,   2,   RNZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX00,   PY00,    PZ02,   2,   RNZFS}, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO61},
	{ {{PX02,   PY02,    PZ00,   3,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	//* 推拿脊椎下
	{ {{PX00,   PY03,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY03,    PZ01,   5,   TNDW }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY04,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY04,    PZ01,   5,   TNDW }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY05,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY05,    PZ02,   5,   TNDW }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY06,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY06,    PZ03,   5,   TNDW }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY07,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY07,    PZ03,   5,   TNDW }, 30    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },	
	
	//* 腰部3D揉捏1 
	{ {{PX00,   PY09,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY09,    PZ02,   2,   D3RNZ}, 200   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},

	//* 腰部3D揉捏2 下
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2,  3},  0xACAC,   0xAAAA}, //* 循环次数X 条数X
	{ {{PX00,   PY09,    PZ02,   2,   RNZ  }, 20    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX03,   PY09,    PZ03,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX03,   PY09,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},

	//* 腰部3D揉捏1 下
    { {{PX00,   PY08,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY08,    PZ03,   2,   D3RNZ}, 200   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},

	//* 腰部3D揉捏2 下
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2,  3},  0xACAC,   0xAAAA}, //* 循环次数X 条数X
	{ {{PX00,   PY08,    PZ02,   2,   RNZ  }, 20    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX03,   PY08,    PZ03,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX03,   PY08,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},

    //* 腰部3D揉捏1 上
	{ {{PX00,   PY07,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY07,    PZ02,   2,   D3RNZ}, 200   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO11},

	//* 腰部3D揉捏2 下
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2,  3},  0xACAC,   0xAAAA}, //* 循环次数X 条数X
	{ {{PX00,   PY07,    PZ01,   2,   RNZ  }, 20    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX03,   PY07,    PZ02,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX03,   PY07,    PZ00,   5,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},

	//* 指压走位上
	{ {{PX00,   PY02,    PZ00,   2,   RNZ  }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	//* 节奏拍打肩部 
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2,  2},  0xACAC,   0xAAAA}, //* 循环次数X 条数X
	{ {{PX00,   PY02,    PZ00,   2,   PDB  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX02,   PY03,    PZ00,   2,   PD   }, 50    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },

	//* 开背向下
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2, 6},  0xACAC,   0xAAAA}, //* 循环次数X 条数X 	
	{ {{PX00,   PY03,    PZ00,   4,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX04,   PY03,    PZ01,   2,   ZY   }, 20    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY04,    PZ00,   4,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX04,   PY04,    PZ01,   2,   ZY   }, 20    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX00,   PY05,    PZ00,   4,   ZY   }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	{ {{PX04,   PY05,    PZ01,   2,   ZY   }, 20    ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },

	{ {{PX00,   PY07,    PZ02,   5,   ZYS  }, 5     ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO9 },
	//* 正常揉捏 腰部
	{ {{PX00,   PY07,    PZ02,   3,   RNZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01}, 
	{ {{PX00,   PY08,    PZ02,   3,   RNZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01}, 
	{ {{PX00,   PY09,    PZ02,   3,   RNZ  }, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01}, 

	//* 腰部 3D 1/2揉捏 下+上
	{ {{PX00,   PY09,    PZ03,   2,   RNZFD}, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY08,    PZ03,   2,   RNZFD}, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO01},
	{ {{PX00,   PY07,    PZ02,   2,   RNZFD}, 100   ,600}, FA_GAS_FULLGAS,      FA_MoveDw_AUTO11},
	 
    //* 揉捏3 腰部
	{ {{0xAA, 0xAA, 0xAA, 0xAA,  0xAA},  2,  2},  0xACAC,   0xAAAA}, //* 循环次数X 条数X 
	{ {{PX00,   PY08,    PZ01,   2,   RNZB }, 100   ,600}, FA_GAS_FULLGAS3,     FA_MoveDw_AUTO01}, 
	{ {{PX00,   PY08,    PZ01,   2,   RNFB }, 100   ,600}, FA_GAS_FULLGAS3,     FA_MoveDw_AUTO01}, 
 * 
 *
 *
==================================================================================================*/
