/*
 * main.cpp
 *
 *  Created on: Jul 24, 2017
 *      Author: devbin
 */

#include "SaApp.h"

int main(int argc, char **argv)
{
    SaApp app;

    int ret = app.start(argc, argv);
    return ret;
}

/*

Evas *e = evas_object_evas_get(getWindowController()->getEvasObject());
Eina_List *fontList = evas_font_available_list(e);
Eina_List *ptr = nullptr;
void *data = nullptr;

EINA_LIST_FOREACH(fontList, ptr, data)
{
	if (data)
	{
		char const *str = (char const *)data;
		WINFO("font : %s", str);
	}
}



static const char *gFontList[] = {
	"SamsungSansNum\-4L:style=Light",
	"SECEmoji:style=Regular",
	"BreezeSansJapanese:style=Regular",
	"BreezeSansGeorgian:style=Regular",
	"SamsungSansNum\-3Lv:style=Light",
	"SamsungOneUIHebrew:style=Medium",
	"BreezeSansMyanmar:style=Regular",
	"Cool jazz:style=Regular",
	"SamsungOneUIFallback:style=Condensed",
	"Rosemary:style=Regular",
	"BreezeSansSinhala:style=Regular",
	"SNum\-3R:style=Regular",
	"Choco cooky:style=Regular",
	"SNum\-3T:style=Thin",
	"BreezeSansGujarathi:style=Regular",
	"SamsungOneUILao:style=Regular",
	"BreezeSansKannada:style=Regular",
	"BreezeSansTibetan:style=Regular",
	"SamsungOneUIKeypad:style=Regular",
	"SamsungOneUIArabic:style=Medium",
	"BreezeSansMeeteiMayek:style=Regular",
	"BreezeSansThai:style=Regular",
	"SamsungOneUIKorean:style=Medium Condensed",
	"SamsungOneUILao:style=Medium",
	"SamsungOneNum:style=Regular",
	"SamsungOneUITCN:style=Regular",
	"SamsungOneUIArabic:style=Regular",
	"BreezeSansTelugu:style=Regular",
	"SamsungOneNum:style=Light",
	"SamsungOneUISCN:style=Regular",
	"SamsungOneUI:style=Regular Condensed",
	"SFitNum:style=Medium Condensed",
	"Clock2017R:style=Regular",
	"BreezeSansOriya:style=Regular",
	"BreezeSansArmenian:style=Medium",
	"SamsungSansNum\-3T:style=Thin",
	"BreezeSansBengali:style=Regular",
	"BreezeSansTamil:style=Regular",
	"BreezeColorEmoji:style=Regular",
	"SamsungOneUIKorean:style=Regular Condensed",
	"SamsungOneUIHebrew:style=Regular",
	"BreezeSansHindi:style=Regular",
	"SNum\-3L:style=Light",
	"BreezeSansKhmer:style=Regular",
	"BreezeSansArmenian:style=Regular",
	"SamsungOneUI:style=Bold Condensed",
	"Moxie:style=Bold",
	"BreezeSansMalayalam:style=Regular",
	"BreezeSansGeorgian:style=Medium",
	"BreezeSansPunjabi:style=Regular",
	"SamsungOneFluid:style=Thin",
	"SamsungOneUIEthiopic:style=Medium",
	"BreezeSansTibetan:style=Medium",
	"SamsungOneNum:style=Thin",
	"SamsungOneUIEthiopic:style=Regular",
	"SamsungOneFluid:style=Regular",
	nullptr
};

*/
