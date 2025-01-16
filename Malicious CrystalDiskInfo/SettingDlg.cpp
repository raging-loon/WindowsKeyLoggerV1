﻿/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : MIT License
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DiskInfo.h"
#include "DiskInfoDlg.h"
#include "SettingDlg.h"

IMPLEMENT_DYNCREATE(CSettingDlg, CDialog)

static CDiskInfoDlg* p;

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogFx(CSettingDlg::IDD, pParent)
{
	p = (CDiskInfoDlg*)pParent;

	m_ZoomType = p->GetZoomType();
	m_FontFace = p->GetFontFace();
	m_FontScale = p->GetFontScale();
	m_FontRatio = p->GetFontRatio();
	m_FontRender = p->GetFontRender();
	m_CurrentLangPath = p->GetCurrentLangPath();
	m_DefaultLangPath = p->GetDefaultLangPath();
	m_ThemeDir = p->GetThemeDir();
	m_CurrentTheme = p->GetCurrentTheme();
	m_DefaultTheme = p->GetDefaultTheme();
	m_Ini = p->GetIniPath();
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogFx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_VALUE_AAM_STATUS, m_AamStatus);
	DDX_Text(pDX, IDC_VALUE_APM_STATUS, m_ApmStatus);
	DDX_Text(pDX, IDC_LABEL_AAM, m_LabelAam);
	DDX_Text(pDX, IDC_LABEL_APM, m_LabelApm);
	DDX_Text(pDX, IDC_LABEL_AAM_LOW, m_AamLow);
	DDX_Text(pDX, IDC_LABEL_AAM_HIGH, m_AamHigh);
	DDX_Text(pDX, IDC_LABEL_AAM_RECOMMEND, m_AamRecommend);
	DDX_Text(pDX, IDC_LABEL_APM_LOW, m_ApmLow);
	DDX_Text(pDX, IDC_LABEL_APM_HIGH, m_ApmHigh);
	DDX_Text(pDX, IDC_VALUE_CURRENT_AAM, m_CurrentAam);
	DDX_Text(pDX, IDC_VALUE_RECOMMEND_AAM, m_RecommendAam);
	DDX_Text(pDX, IDC_VALUE_CURRENT_APM, m_CurrentApm);
	DDX_Text(pDX, IDC_BUTTON_ENABLE_AAM, m_EnableAam);
	DDX_Text(pDX, IDC_BUTTON_DISABLE_AAM, m_DisableAam);
	DDX_Text(pDX, IDC_BUTTON_ENABLE_APM, m_EnableApm);
	DDX_Text(pDX, IDC_BUTTON_DISABLE_APM, m_DisableApm);

	DDX_Control(pDX, IDC_VALUE_AAM_STATUS, m_CtrlAamStatus);
	DDX_Control(pDX, IDC_VALUE_APM_STATUS, m_CtrlApmStatus);
	DDX_Control(pDX, IDC_LABEL_AAM, m_CtrlLabelAam);
	DDX_Control(pDX, IDC_LABEL_APM, m_CtrlLabelApm);
	DDX_Control(pDX, IDC_LABEL_AAM_LOW, m_CtrlLabelAamLow);
	DDX_Control(pDX, IDC_LABEL_AAM_HIGH, m_CtrlLabelAamHigh);
	DDX_Control(pDX, IDC_LABEL_AAM_RECOMMEND, m_CtrlLabelAamRecommend);
	DDX_Control(pDX, IDC_LABEL_APM_LOW, m_CtrlLabelApmLow);
	DDX_Control(pDX, IDC_LABEL_APM_HIGH, m_CtrlLabelApmHigh);
	DDX_Control(pDX, IDC_VALUE_CURRENT_AAM, m_CtrlCurrentAam);
	DDX_Control(pDX, IDC_VALUE_RECOMMEND_AAM, m_CtrlRecommendAam);
	DDX_Control(pDX, IDC_VALUE_CURRENT_APM, m_CtrlCurrentApm);
	DDX_Control(pDX, IDC_BUTTON_ENABLE_AAM, m_CtrlEnableAam);
	DDX_Control(pDX, IDC_BUTTON_DISABLE_AAM, m_CtrlDisableAam);
	DDX_Control(pDX, IDC_BUTTON_ENABLE_APM, m_CtrlEnableApm);
	DDX_Control(pDX, IDC_BUTTON_DISABLE_APM, m_CtrlDisableApm);
	DDX_Control(pDX, IDC_SELECT_DISK, m_CtrlSelectDisk);

	DDX_Control(pDX, IDC_AAM_SCROLLBAR, m_AamScrollbar);
	DDX_Control(pDX, IDC_APM_SCROLLBAR, m_ApmScrollbar);
}

BOOL CSettingDlg::OnInitDialog()
{
	CDialogFx::OnInitDialog();

	SetWindowTitle(i18n(_T("WindowTitle"), _T("AAM_APM_CONTROL")));

	m_AamScrollbar.SetScrollRange(0x80, 0xFE);
	m_ApmScrollbar.SetScrollRange(0x01, 0xFE);

	m_bShowWindow = TRUE;
	InitLang();
	InitSelectDisk();

	UpdateDialogSize();
	
	CenterWindow();
	ShowWindow(SW_SHOW);

	if(p->m_Ata.vars.GetCount() == 0)
	{
		m_CtrlSelectDisk.EnableWindow(FALSE);

		m_AamScrollbar.EnableWindow(FALSE);
		m_ApmScrollbar.EnableWindow(FALSE);

		m_CtrlLabelAam.EnableWindow(FALSE);
		m_CtrlLabelApm.EnableWindow(FALSE);

		m_CtrlLabelAamLow.EnableWindow(FALSE);
		m_CtrlLabelAamHigh.EnableWindow(FALSE);
		m_CtrlLabelAamRecommend.EnableWindow(FALSE);
		m_CtrlLabelApmLow.EnableWindow(FALSE);
		m_CtrlLabelApmHigh.EnableWindow(FALSE);

		m_CtrlAamStatus.EnableWindow(FALSE);
		m_CtrlCurrentAam.EnableWindow(FALSE);
		m_CtrlRecommendAam.EnableWindow(FALSE);
		m_CtrlApmStatus.EnableWindow(FALSE);
		m_CtrlCurrentApm.EnableWindow(FALSE);

		m_CtrlEnableAam.EnableWindow(FALSE);
		m_CtrlDisableAam.EnableWindow(FALSE);
		m_CtrlEnableApm.EnableWindow(FALSE);
		m_CtrlDisableApm.EnableWindow(FALSE);
	}
	return TRUE;
}

void CSettingDlg::UpdateDialogSize()
{
	CDialogFx::UpdateDialogSize();

	ChangeZoomType(m_ZoomType);
	SetClientSize(SIZE_X, SIZE_Y, m_ZoomRatio);
	UpdateBackground(FALSE, m_bDarkMode);

	m_CtrlLabelAam.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlLabelApm.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);

	m_CtrlLabelAamLow.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlLabelAamHigh.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlLabelAamRecommend.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlLabelApmLow.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlLabelApmHigh.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);

	m_CtrlAamStatus.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlCurrentAam.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlRecommendAam.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlApmStatus.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlCurrentApm.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);

	m_CtrlEnableAam.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlDisableAam.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlEnableApm.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);
	m_CtrlDisableApm.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio, RGB(0, 0, 0), FW_NORMAL, m_FontRender);

	m_CtrlLabelAam.InitControl(8, 44, 384, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, CStaticFx::Border::UNDERLINE);
	m_CtrlLabelApm.InitControl(8, 188, 384, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, CStaticFx::Border::UNDERLINE);

	m_CtrlLabelAamLow.InitControl(16, 72, 160, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlLabelAamHigh.InitControl(176, 72, 160, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_RIGHT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlLabelAamRecommend.InitControl(176, 120, 160, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_RIGHT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlLabelApmLow.InitControl(16, 216, 160, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlLabelApmHigh.InitControl(176, 216, 160, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_RIGHT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, FALSE);

	m_CtrlAamStatus.InitControl(344, 72, 40, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, TRUE);
	m_CtrlCurrentAam.InitControl(344, 96, 40, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, TRUE);
	m_CtrlRecommendAam.InitControl(344, 120, 40, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, TRUE);
	m_CtrlApmStatus.InitControl(344, 216, 40, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, TRUE);
	m_CtrlCurrentApm.InitControl(344, 240, 40, 20, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, TRUE);

	m_CtrlEnableAam.InitControl(220, 148, 160, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlDisableAam.InitControl(20, 148, 160, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlEnableApm.InitControl(220, 272, 160, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode, FALSE);
	m_CtrlDisableApm.InitControl(20, 272, 160, 24, m_ZoomRatio, &m_BkDC, NULL, 0, SS_CENTER, SystemDraw, m_bHighContrast, m_bDarkMode, FALSE);

	m_CtrlSelectDisk.SetFontEx(m_FontFace, 12, 12, m_ZoomRatio, m_FontRatio);
	m_CtrlSelectDisk.InitControl(8, 8, 384, 40, m_ZoomRatio, &m_BkDC, NULL, 0, ES_LEFT, OwnerDrawTransparent, m_bHighContrast, m_bDarkMode, RGB(255, 255, 255), RGB(160, 220, 255), RGB(255, 255, 255), 0);
	m_CtrlSelectDisk.SetMargin(0, 4, 0, 0, m_ZoomRatio);

	m_AamScrollbar.MoveWindow((DWORD)(16 * m_ZoomRatio), (DWORD)(96 * m_ZoomRatio), (DWORD)(324 * m_ZoomRatio), (DWORD)(20 * m_ZoomRatio));
	m_ApmScrollbar.MoveWindow((DWORD)(16 * m_ZoomRatio), (DWORD)(240 * m_ZoomRatio), (DWORD)(324 * m_ZoomRatio), (DWORD)(20 * m_ZoomRatio));

	m_CtrlAamStatus.SetDrawFrame(TRUE);
	m_CtrlCurrentAam.SetDrawFrame(TRUE);
	m_CtrlRecommendAam.SetDrawFrame(TRUE);
	m_CtrlApmStatus.SetDrawFrame(TRUE);
	m_CtrlCurrentApm.SetDrawFrame(TRUE);

	m_CtrlEnableAam.SetHandCursor();
	m_CtrlDisableAam.SetHandCursor();
	m_CtrlEnableApm.SetHandCursor();
	m_CtrlDisableApm.SetHandCursor();

	SetDarkModeControl(m_CtrlEnableAam.GetSafeHwnd(), m_bDarkMode);
	SetDarkModeControl(m_CtrlDisableAam.GetSafeHwnd(), m_bDarkMode);
	SetDarkModeControl(m_CtrlEnableApm.GetSafeHwnd(), m_bDarkMode);
	SetDarkModeControl(m_CtrlDisableApm.GetSafeHwnd(), m_bDarkMode);
	SetDarkModeControl(m_AamScrollbar.GetSafeHwnd(), m_bDarkMode);
	SetDarkModeControl(m_ApmScrollbar.GetSafeHwnd(), m_bDarkMode);

	// SetLayeredWindow(m_CtrlSelectDisk.GetListHwnd(), m_ComboAlpha);
	for (int i = -1; i < m_CtrlSelectDisk.GetCount(); i++)
	{
		m_CtrlSelectDisk.SetItemHeightEx(i, 24, m_ZoomRatio, m_FontRatio);
	}

	Invalidate();
}

BEGIN_MESSAGE_MAP(CSettingDlg, CDialogFx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_ENABLE_AAM, &CSettingDlg::OnEnableAam)
	ON_BN_CLICKED(IDC_BUTTON_ENABLE_APM, &CSettingDlg::OnEnableApm)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE_AAM, &CSettingDlg::OnDisableAam)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE_APM, &CSettingDlg::OnDisableApm)
	ON_CBN_SELCHANGE(IDC_SELECT_DISK, &CSettingDlg::OnCbnSelchangeSelectDisk)
END_MESSAGE_MAP()

void CSettingDlg::InitLang()
{
	m_AamLow = i18n(_T("AamApm"), _T("AAM_LOW"));
	m_AamHigh = i18n(_T("AamApm"), _T("AAM_HIGH"));
	m_AamRecommend = i18n(_T("AamApm"), _T("AAM_RECOMMEND"));

	m_ApmLow = i18n(_T("AamApm"), _T("APM_LOW"));
	m_ApmHigh = i18n(_T("AamApm"), _T("APM_HIGH"));

	m_LabelAam = i18n(_T("AamApm"), _T("AUTOMATIC_ACOUSTIC_MANAGEMENT"));
	m_LabelApm = i18n(_T("AamApm"), _T("ADVANCED_POWER_MANAGEMENT"));

	m_EnableAam = i18n(_T("TrayMenu"), _T("ENABLE"));
	m_DisableAam = i18n(_T("TrayMenu"), _T("DISABLE"));
	m_EnableApm = i18n(_T("TrayMenu"), _T("ENABLE"));
	m_DisableApm = i18n(_T("TrayMenu"), _T("DISABLE"));

	UpdateData(FALSE);
}

void CSettingDlg::InitSelectDisk()
{
	CString select;
	CString cstr;

	if(p->m_Ata.vars.GetCount() <= 0)
	{
		return ;
	}

	for(int i = 0; i < p->m_Ata.vars.GetCount(); i++)
	{
		CString temp;
		if(p->m_Ata.vars.GetAt(i).IsAamSupported && p->m_Ata.vars.GetAt(i).IsApmSupported)
		{
			temp = _T("[AAM, APM]");	
		}
		else if(p->m_Ata.vars.GetAt(i).IsAamSupported)
		{
			temp = _T("[AAM]");	
		}
		else if(p->m_Ata.vars.GetAt(i).IsApmSupported)
		{
			temp = _T("[APM]");	
		}

		CString driveLetter;
		if (p->m_Ata.vars[i].DriveMap.IsEmpty())
		{
			if (p->m_Ata.vars[i].PhysicalDriveId >= 0)
			{
				driveLetter.Format(_T("(Disk %d)"), p->m_Ata.vars[i].PhysicalDriveId);
			}
		}
		else
		{
			driveLetter.Format(_T("(%s)"), p->m_Ata.vars[i].DriveMap.GetString());
		}

		cstr.Format(_T("(%02d) %s %s %s"), i + 1, p->m_Ata.vars.GetAt(i).Model.GetString(), temp.GetString(), driveLetter.GetString());
		m_CtrlSelectDisk.AddString(cstr);

		if (i == p->GetSelectedDrive())
		{
			m_CtrlSelectDisk.SetCurSel(i);
			m_DiskIndex = i;
		}
	}

	UpdateSelectDisk(m_DiskIndex);
}

/* Memo 2008/11/8
AamStatus/ApmStatus
1 : Enabled
0 : Disabled
-1: Unsupported
*/

void CSettingDlg::OnEnableAam()
{
	if(! p->m_Ata.vars.GetAt(m_DiskIndex).IsAamSupported)
	{
		return ;
	}

	int targetValue = m_AamScrollbar.GetScrollPos();
	p->m_Ata.EnableAam(m_DiskIndex, (BYTE)targetValue);
	p->m_Ata.UpdateIdInfo(m_DiskIndex);

	if(p->m_Ata.vars.GetAt(m_DiskIndex).IsAamEnabled)
	{
		m_AamStatus = _T("ON");
	}
	m_AamScrollbar.SetScrollPos(p->m_Ata.GetAamValue(m_DiskIndex));
	m_CurrentAam.Format(_T("%02Xh"), p->m_Ata.GetAamValue(m_DiskIndex));
	m_RecommendAam.Format(_T("%02Xh"), p->m_Ata.GetRecommendAamValue(m_DiskIndex));
	UpdateData(FALSE);

	// Save Settings
	CString cstr;
	cstr.Format(_T("%d"), targetValue);
	WritePrivateProfileStringFx(_T("AamStatus"), p->m_Ata.vars[m_DiskIndex].ModelSerial, _T("1"), m_Ini);
	WritePrivateProfileStringFx(_T("AamValue"), p->m_Ata.vars[m_DiskIndex].ModelSerial, cstr, m_Ini);
}

void  CSettingDlg::OnDisableAam()
{
	if(! p->m_Ata.vars.GetAt(m_DiskIndex).IsAamSupported)
	{
		return ;
	}

	p->m_Ata.DisableAam(m_DiskIndex);
	p->m_Ata.UpdateIdInfo(m_DiskIndex);

	if(! p->m_Ata.vars.GetAt(m_DiskIndex).IsAamEnabled)
	{
		m_AamStatus = _T("OFF");
	}
	m_AamScrollbar.SetScrollPos(p->m_Ata.GetAamValue(m_DiskIndex));
	m_CurrentAam.Format(_T("%02Xh"), p->m_Ata.GetAamValue(m_DiskIndex));
	m_RecommendAam.Format(_T("%02Xh"), p->m_Ata.GetRecommendAamValue(m_DiskIndex));
	UpdateData(FALSE);

	// Save Settings
//	CString cstr;
//	cstr.Format(_T("%d"), m_AamScrollbar.GetScrollPos());
	WritePrivateProfileStringFx(_T("AamStatus"), p->m_Ata.vars[m_DiskIndex].ModelSerial, _T("0"), m_Ini);
//	WritePrivateProfileStringFx(_T("AamValue"), p->m_Ata.vars[m_DiskIndex].ModelSerial, cstr, m_Ini);
}

void  CSettingDlg::OnEnableApm()
{
	if(! p->m_Ata.vars.GetAt(m_DiskIndex).IsApmSupported)
	{
		return ;
	}

	int targetValue = m_ApmScrollbar.GetScrollPos();
	p->m_Ata.EnableApm(m_DiskIndex, (BYTE)targetValue);
	p->m_Ata.UpdateIdInfo(m_DiskIndex);

	if(p->m_Ata.vars.GetAt(m_DiskIndex).IsApmEnabled)
	{
		m_ApmStatus = _T("ON");
	}
	m_ApmScrollbar.SetScrollPos(p->m_Ata.GetApmValue(m_DiskIndex));
	m_CurrentApm.Format(_T("%02Xh"), p->m_Ata.GetApmValue(m_DiskIndex));
	UpdateData(FALSE);

	// Save Settings
	CString cstr;
	cstr.Format(_T("%d"), targetValue);
	WritePrivateProfileStringFx(_T("ApmStatus"), p->m_Ata.vars[m_DiskIndex].ModelSerial, _T("1"), m_Ini);
	WritePrivateProfileStringFx(_T("ApmValue"), p->m_Ata.vars[m_DiskIndex].ModelSerial, cstr, m_Ini);
}

void  CSettingDlg::OnDisableApm()
{
	if(! p->m_Ata.vars.GetAt(m_DiskIndex).IsApmSupported)
	{
		return ;
	}

	p->m_Ata.DisableApm(m_DiskIndex);
	p->m_Ata.UpdateIdInfo(m_DiskIndex);

	if(! p->m_Ata.vars.GetAt(m_DiskIndex).IsApmEnabled)
	{
		m_ApmStatus = _T("OFF");
	}
	m_ApmScrollbar.SetScrollPos(p->m_Ata.GetApmValue(m_DiskIndex));
	m_CurrentApm.Format(_T("%02Xh"), p->m_Ata.GetApmValue(m_DiskIndex));
	UpdateData(FALSE);

	// Save Settings
//	CString cstr;
//	cstr.Format(_T("%d"), m_ApmScrollbar.GetScrollPos());
	WritePrivateProfileStringFx(_T("ApmStatus"), p->m_Ata.vars[m_DiskIndex].ModelSerial, _T("0"), m_Ini);
//	WritePrivateProfileStringFx(_T("ApmValue"), p->m_Ata.vars[m_DiskIndex].ModelSerial, cstr, m_Ini);
}

void CSettingDlg::UpdateSelectDisk(DWORD index)
{
	m_AamScrollbar.SetScrollPos(p->m_Ata.GetAamValue(index));
	m_CurrentAam.Format(_T("%02Xh"), p->m_Ata.GetAamValue(index));
	m_RecommendAam.Format(_T("%02Xh"), p->m_Ata.GetRecommendAamValue(index));

	m_ApmScrollbar.SetScrollPos(p->m_Ata.GetApmValue(index));
	m_CurrentApm.Format(_T("%02Xh"), p->m_Ata.GetApmValue(index));

	if(p->m_Ata.vars.GetAt(index).IsAamSupported 
		&& p->m_Ata.vars.GetAt(index).CommandType != p->m_Ata.CMD_TYPE_SCSI_MINIPORT
		&& p->m_Ata.vars.GetAt(index).CommandType != p->m_Ata.CMD_TYPE_SILICON_IMAGE
		&& p->m_Ata.vars.GetAt(index).CommandType != p->m_Ata.CMD_TYPE_CSMI
		)
	{
		m_AamScrollbar.EnableWindow(TRUE);
		m_CtrlEnableAam.EnableWindow(TRUE);
		m_CtrlDisableAam.EnableWindow(TRUE);
		if(p->m_Ata.vars.GetAt(index).IsAamEnabled)
		{
			m_AamStatus = _T("ON");
		}
		else
		{
			m_AamStatus = _T("OFF");
		}
	}
	else
	{
		m_AamScrollbar.EnableWindow(FALSE);
		m_CtrlEnableAam.EnableWindow(FALSE);
		m_CtrlDisableAam.EnableWindow(FALSE);
		m_AamStatus = _T("");
		m_CurrentAam = _T("");
		m_RecommendAam = _T("");
	}

	if(p->m_Ata.vars.GetAt(index).IsApmSupported
		&& p->m_Ata.vars.GetAt(index).CommandType != p->m_Ata.CMD_TYPE_SCSI_MINIPORT
		&& p->m_Ata.vars.GetAt(index).CommandType != p->m_Ata.CMD_TYPE_SILICON_IMAGE
		&& p->m_Ata.vars.GetAt(index).CommandType != p->m_Ata.CMD_TYPE_CSMI
		)
	{
		m_ApmScrollbar.EnableWindow(TRUE);
		m_CtrlEnableApm.EnableWindow(TRUE);
		m_CtrlDisableApm.EnableWindow(TRUE);
		if(p->m_Ata.vars.GetAt(index).IsApmEnabled)
		{
			m_ApmStatus = _T("ON");
		}
		else
		{
			m_ApmStatus = _T("OFF");
		}
	}
	else
	{
		m_ApmScrollbar.EnableWindow(FALSE);
		m_CtrlEnableApm.EnableWindow(FALSE);
		m_CtrlDisableApm.EnableWindow(FALSE);
		m_ApmStatus = _T("");
		m_CurrentApm = _T("");
	}

	UpdateData(FALSE);
}

void CSettingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int position = pScrollBar->GetScrollPos();
	switch(nSBCode)
	{

	case SB_LINELEFT:
		position -= 1;
		break;
	case SB_LINERIGHT:
		position += 1;
		break;
	case SB_PAGELEFT:
		position -= 8;
		break;
	case SB_PAGERIGHT:
		position += 8;
		break;
	case SB_LEFT:
		break;
	case SB_RIGHT:
		break;
	case SB_THUMBTRACK:
		position = nPos;
		break;
	}
	pScrollBar->SetScrollPos(position);

	if(*pScrollBar == m_AamScrollbar)
	{
		m_CurrentAam.Format(_T("%02Xh"), m_AamScrollbar.GetScrollPos());
	}
	else if(*pScrollBar == m_ApmScrollbar)
	{
		m_CurrentApm.Format(_T("%02Xh"), m_ApmScrollbar.GetScrollPos());
	}

	UpdateData(FALSE);
	
	CDialogFx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CSettingDlg::OnCbnSelchangeSelectDisk()
{
	if(m_DiskIndex != m_CtrlSelectDisk.GetCurSel())
	{
		m_DiskIndex = m_CtrlSelectDisk.GetCurSel();
		UpdateSelectDisk(m_DiskIndex);
	}
}
