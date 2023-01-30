// 11 april 2015
#include "uipriv_windows.hpp"

struct uiLabel {
	uiWindowsControl c;
	HWND hwnd;
	bool cache_valid;
	int cached_width;
	int cached_height;
};

uiWindowsControlAllDefaults(uiLabel)

static void uiLabelMinimumSizeImpl(uiLabel* l)
{
	uiWindowsSizing sizing;
	int y;

	l->cached_width = uiWindowsWindowTextWidth(l->hwnd);
	y = uiWindowsWindowTextHeight(l->hwnd);
	uiWindowsGetSizing(l->hwnd, &sizing);
	uiWindowsSizingDlgUnitsToPixels(&sizing, NULL, &y);
	l->cached_height = y;
}

static void uiLabelMinimumSize(uiWindowsControl* c, int* width, int* height)
{
	uiLabel* l = uiLabel(c);
	if (!l->cache_valid)
	{
		uiLabelMinimumSizeImpl(l);
		l->cache_valid = true;
	}

	*width = l->cached_width;
	*height = l->cached_height;
}

char *uiLabelText(uiLabel *l)
{
	return uiWindowsWindowText(l->hwnd);
}

void uiLabelSetText(uiLabel *l, const char *text)
{
	uiWindowsSetWindowText(l->hwnd, text);
	l->cache_valid = false;
	// changing the text might necessitate a change in the label's size
	uiWindowsControlMinimumSizeChanged(uiWindowsControl(l));
}

uiLabel *uiNewLabel(const char *text)
{
	uiLabel *l;
	WCHAR *wtext;

	uiWindowsNewControl(uiLabel, l);

	wtext = toUTF16(text);
	l->hwnd = uiWindowsEnsureCreateControlHWND(0,
		L"static", wtext,
		// SS_LEFTNOWORDWRAP clips text past the end; SS_NOPREFIX avoids accelerator translation
		// controls are vertically aligned to the top by default (thanks Xeek in irc.freenode.net/#winapi)
		SS_LEFTNOWORDWRAP | SS_NOPREFIX,
		hInstance, NULL,
		TRUE);
	l->cache_valid = false;
	uiprivFree(wtext);

	return l;
}
