#include "stdafx.h"
#include "UnifLoader.h"

std::unordered_map<string, int> UnifLoader::_boardMappings = std::unordered_map<string, int> {
	{ "11160", 299 },
	{ "12-IN-1", 331 },
	{ "13in1JY110", UnifBoards::UnknownBoard },
	{ "190in1", 300 },
	{ "22211", 132 },
	{ "255in1", UnifBoards::Unl255in1 }, //Doesn't actually exist as a UNIF file (used to assign a mapper to the 255-in-1 rom)
	{ "3D-BLOCK", UnifBoards::UnknownBoard },
	{ "411120-C", 287 },
	{ "42in1ResetSwitch", 226 },
	{ "43272", 227 },
	{ "603-5052", 238 },
	{ "64in1NoRepeat", 314 },
	{ "70in1", 236 },
	{ "70in1B", 236 },
	{ "810544-C-A1", 261 },
	{ "830425C-4391T", 320 },
	{ "8157", 301 },
	{ "8237", 215 },
	{ "8237A", UnifBoards::Unl8237A },
	{ "830118C", 348 },
	{ "A65AS", 285 },
	{ "AC08", UnifBoards::Ac08 },
	{ "ANROM", 7 },
	{ "AX5705", 530 },
	{ "BB", 108 },
	{ "BS-5", 286 },
	{ "CC-21", UnifBoards::Cc21 },
	{ "CITYFIGHT", 266 },
	{ "COOLBOY", 268 },
	{ "10-24-C-A1", UnifBoards::UnknownBoard },
	{ "CNROM", 3 },
	{ "CPROM", 13 },
	{ "D1038", 59 },
	{ "DANCE", UnifBoards::UnknownBoard },
	{ "DANCE2000", 518 },
	{ "DREAMTECH01", 521 },
	{ "EDU2000", 329 },
	{ "EKROM", 5 },
	{ "ELROM", 5 },
	{ "ETROM", 5 },
	{ "EWROM", 5 },
	{ "FARID_SLROM_8-IN-1", 323 },
	{ "FARID_UNROM_8-IN-1", 324 },
	{ "FK23C", 176 },
	{ "FK23CA", 176 },
	{ "FS304", 162 },
	{ "G-146", 349 },
	{ "GK-192", 58 },
	{ "GS-2004", 283 },
	{ "GS-2013", UnifBoards::Gs2013 },
	{ "Ghostbusters63in1", UnifBoards::Ghostbusters63in1 },
	{ "H2288", 123 },
	{ "HKROM", 4 },
	{ "KOF97", 263 },
	{ "KONAMI-QTAI", 190 },
	{ "K-3046", 336 },
	{ "KS7010", UnifBoards::UnknownBoard },
	{ "KS7012", 346 },
	{ "KS7013B", 312 },
	{ "KS7016", 306 },
	{ "KS7017", 303 },
	{ "KS7030", UnifBoards::UnknownBoard },
	{ "KS7031", 305 },
	{ "KS7032", 142 },
	{ "KS7037", 307 },
	{ "KS7057", 302 },
	{ "LE05", UnifBoards::UnknownBoard },
	{ "LH10", 522 },
	{ "LH32", 125 },
	{ "LH51", 309 },
	{ "LH53", UnifBoards::UnknownBoard },
	{ "MALISB", 325 },
	{ "MARIO1-MALEE2", UnifBoards::Malee },
	{ "MHROM", 66 },
	{ "N625092", 221 },
	{ "NROM", 0 },
	{ "NROM-128", 0 },
	{ "NROM-256", 0 },
	{ "NTBROM", 68 },
	{ "NTD-03", 290 },
	{ "NovelDiamond9999999in1", 201 },
	{ "OneBus", UnifBoards::UnknownBoard },
	{ "PEC-586", UnifBoards::UnknownBoard },
	{ "PUZZLE", UnifBoards::UnlPuzzle }, //Doesn't actually exist as a UNIF file (used to reassign a new mapper number to the Puzzle beta)
	{ "RESET-TXROM", 313 },
	{ "RET-CUFROM", 29 },
	{ "RROM", 0 },
	{ "RROM-128", 0 },
	{ "SA-002", 136 },
	{ "SA-0036", 149 },
	{ "SA-0037", 148 },
	{ "SA-009", 160 },
	{ "SA-016-1M", 146 },
	{ "SA-72007", 145 },
	{ "SA-72008", 133 },
	{ "SA-9602B", 513 },
	{ "SA-NROM", 143 },
	{ "SAROM", 1 },
	{ "SBROM", 1 },
	{ "SC-127", 35 },
	{ "SCROM", 1 },
	{ "SEROM", 1 },
	{ "SGROM", 1 },
	{ "SHERO", 262 },
	{ "SKROM", 1 },
	{ "SL12", 116 },
	{ "SL1632", 14 },
	{ "SL1ROM", 1 },
	{ "SLROM", 1 },
	{ "SMB2J", 304 },
	{ "SNROM", 1 },
	{ "SOROM", 1 },
	{ "SSS-NROM-256", UnifBoards::SssNrom256 },
	{ "SUNSOFT_UNROM", 93 },
	{ "Sachen-74LS374N", 150 },
	{ "Sachen-74LS374NA", 243 },
	{ "Sachen-8259A", 141 },
	{ "Sachen-8259B", 138 },
	{ "Sachen-8259C", 139 },
	{ "Sachen-8259D", 137 },
	{ "Super24in1SC03", 176 },
	{ "SuperHIK8in1", 45 },
	{ "Supervision16in1", 53 },
	{ "T-227-1", UnifBoards::UnknownBoard },
	{ "T-230", 529 },
	{ "T-262", 265 },
	{ "TBROM", 4 },
	{ "TC-U01-1.5M", 147 },
	{ "TEK90", 90 },
	{ "TEROM", 4 },
	{ "TF1201", 298 },
	{ "TFROM", 4 },
	{ "TGROM", 4 },
	{ "TKROM", 4 },
	{ "TKSROM", 4 },
	{ "TLROM", 4 },
	{ "TLSROM", 4 },
	{ "TQROM", 4 },
	{ "TR1ROM", 4 },
	{ "TSROM", 4 },
	{ "TVROM", 4 },
	{ "Transformer", UnifBoards::UnknownBoard },
	{ "UNROM", 2 },
	{ "UNROM-512-8", 30 },
	{ "UNROM-512-16", 30 },
	{ "UNROM-512-32", 30 },
	{ "UOROM", 2 },
	{ "VRC7", 85 },
	{ "YOKO", 264 },
	{ "SB-2000", UnifBoards::UnknownBoard },
	{ "158B", 258 },
	{ "DRAGONFIGHTER", 292 },
	{ "EH8813A", 519 },
	{ "HP898F", 319 },
	{ "F-15", 259 },
	{ "RT-01", 328 },
	{ "81-01-31-C", UnifBoards::UnknownBoard },
	{ "8-IN-1", 333 },
	{ "WS", 332 },
	{ "80013-B", 274 },
	{ "WAIXING-FW01", 227 },
	{ "WAIXING-FS005", UnifBoards::UnknownBoard },
	{ "HPxx", 260 },
	{ "HP2018A", 260 },
	{ "DRIPGAME", 284 },
	{ "60311C", 289 },
	{ "CHINA_ER_SAN2", 19 }, //Appears to be a mapper 19 hack specific for VirtuaNES (which adds chinese text on top of the PPU's output), unknown if a board actually exists
};