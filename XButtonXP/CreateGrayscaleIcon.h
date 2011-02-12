// CreateGrayscaleIcon.h
//
// CreateGrayscaleIcon which supports 24 and 32 bpp icons by Gladstone
// with fixes by poxui.
// http://www.codeproject.com/KB/graphics/Create_GrayscaleIcon.aspx
//
///////////////////////////////////////////////////////////////////////////////

#ifndef CREATEGRAYSCALEICON_H
#define CREATEGRAYSCALEICON_H

static COLORREF defaultGrayPalette[256];
static BOOL bGrayPaletteSet = FALSE;

HICON CreateGrayscaleIcon(HICON hIcon, COLORREF* pPalette)
{
	HICON hGrayIcon = NULL;

	ASSERT(hIcon);

	if (hIcon == NULL)
		return NULL;

	HDC hdc = ::GetDC(NULL);

	BITMAPINFO bmpInfo = { 0 };
	bmpInfo.bmiHeader.biSize  = sizeof(BITMAPINFOHEADER);

	ICONINFO icInfo = { 0 };
	if (::GetIconInfo(hIcon, &icInfo))
	{
		if (::GetDIBits(hdc, icInfo.hbmColor, 0, 0, NULL, &bmpInfo, 
				DIB_RGB_COLORS) != 0)
		{
			const int size = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 
				bmpInfo.bmiHeader.biClrUsed;
			BITMAPINFO* pBmpInfo = (BITMAPINFO*)new BYTE[size+100];
			memset(pBmpInfo, 0, size+100);
			pBmpInfo->bmiHeader = bmpInfo.bmiHeader;
			pBmpInfo->bmiHeader.biCompression = BI_RGB;
			
			LPDWORD lpBits = NULL;
			if (::GetDIBits(hdc, icInfo.hbmColor, 0, bmpInfo.bmiHeader.biHeight, 
					lpBits, pBmpInfo, DIB_RGB_COLORS) != 0)
			{
				bmpInfo.bmiHeader.biPlanes = 1;
				bmpInfo.bmiHeader.biBitCount = 32;
				bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth * 4 * 
					bmpInfo.bmiHeader.biHeight;
				bmpInfo.bmiHeader.biClrUsed = 0;
				bmpInfo.bmiHeader.biClrImportant = 0;
				bmpInfo.bmiHeader.biCompression = BI_RGB;

				SIZE sz;
				sz.cx = bmpInfo.bmiHeader.biWidth;
				sz.cy = bmpInfo.bmiHeader.biHeight;
				DWORD c1 = sz.cx * sz.cy;
	
				lpBits = (LPDWORD)::GlobalAlloc(GMEM_FIXED, c1 * 4);
	
				if (lpBits && ::GetDIBits(hdc, icInfo.hbmColor, 0, sz.cy, 
						lpBits, &bmpInfo, DIB_RGB_COLORS) != 0)
				{
					LPBYTE lpBitsPtr = (LPBYTE)lpBits;
					UINT off  = 0;
	
					for (UINT i = 0; i < c1; i++)
					{
						DWORD dwBits = lpBitsPtr[0] + lpBitsPtr[1] + lpBitsPtr[2];
						off = (UINT)(255 - (dwBits / 3));
	
						if (lpBitsPtr[3] != 0 || off != 255)
						{
							if (off == 0)
								off = 1;
	
							lpBits[i] = pPalette[off] | (lpBitsPtr[3] << 24);
						}
	
						lpBitsPtr += 4;
					}
	
					ICONINFO icGrayInfo = { 0 };
					icGrayInfo.hbmColor = ::CreateCompatibleBitmap(hdc, sz.cx, sz.cy);
	
					if (icGrayInfo.hbmColor != NULL)
					{
						::SetDIBits(hdc, icGrayInfo.hbmColor, 0, sz.cy, lpBits, 
							&bmpInfo, DIB_RGB_COLORS);
	
						icGrayInfo.hbmMask = icInfo.hbmMask;
						icGrayInfo.fIcon   = TRUE;
	
						hGrayIcon = ::CreateIconIndirect(&icGrayInfo);
	
						::DeleteObject(icGrayInfo.hbmColor);
					}
	
					::GlobalFree(lpBits);
					lpBits = NULL;
				}
			}
			delete [] pBmpInfo;
		}

		::DeleteObject(icInfo.hbmColor);
		::DeleteObject(icInfo.hbmMask);
	}

	::ReleaseDC(NULL, hdc);

	return hGrayIcon;
}

HICON CreateGrayscaleIcon(HICON hIcon)
{
	HICON hGrayIcon = NULL;

	ASSERT(hIcon);

	if (hIcon)
	{
		if (!bGrayPaletteSet)
		{
			for (int i = 0; i < 256; i++)
			{
				defaultGrayPalette[i] = RGB(255-i, 255-i, 255-i);
			}

			bGrayPaletteSet = TRUE;
		}

		hGrayIcon = CreateGrayscaleIcon(hIcon, defaultGrayPalette);
	}

	return hGrayIcon;
}

#endif
