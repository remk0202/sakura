/*!	@file
	@brief ���ʐݒ�_�C�A���O�{�b�N�X�A�u�S�ʁv�y�[�W

	@author Norio Nakatani
	@date 1998/12/24 �V�K�쐬
*/
/*
	Copyright (C) 1998-2002, Norio Nakatani
	Copyright (C) 2000-2001, jepro
	Copyright (C) 2001, genta, MIK, hor, Stonee, YAZAKI
	Copyright (C) 2002, YAZAKI, aroka, MIK, Moca, ������
	Copyright (C) 2003, MIK, KEITA
	Copyright (C) 2006, ryoji
	Copyright (C) 2007, genta, ryoji, Moca, nasukoji
	Copyright (C) 2008, Uchi
	Copyright (C) 2009, nasukoji

	This source code is designed for sakura editor.
	Please contact the copyright holders to use this code for other purpose.
*/

#include "StdAfx.h"
#include "CPropCommon.h"
#include "Debug.h"
#include <windows.h>
#include <commctrl.h>
#include "CDlgOpenFile.h"
#include "etc_uty.h"
#include "global.h"
#include "CDlgInput1.h"
#include "CDlgDebug.h"
#include "CSplitBoxWnd.h"
#include "CMenuDrawer.h"
#include "Funccode.h"	//Stonee, 2001/05/18
#include "CEditApp.h"
#include "sakura_rc.h"
#include "sakura.hh"

//@@@ 2001.02.04 Start by MIK: Popup Help

// 2009.01.12 nasukoji	�I�v�V�����̎擾�E�ݒ�p�iCPropTypes.cpp����R�s�[�j
struct TYPE_NAME {
	int		nMethod;
	char*	pszName;
};

TYPE_NAME SpecialScrollModeArr[] = {
	{ 0,						_T("�g�ݍ��킹�Ȃ�") },
	{ MOUSEFUNCTION_CENTER,		_T("�}�E�X���{�^��") },
	{ MOUSEFUNCTION_LEFTSIDE,	_T("�}�E�X�T�C�h�{�^��1") },
	{ MOUSEFUNCTION_RIGHTSIDE,	_T("�}�E�X�T�C�h�{�^��2") },
	{ VK_CONTROL,				_T("CONTROL�L�[") },
	{ VK_SHIFT,					_T("SHIFT�L�[") },
};

static const DWORD p_helpids[] = {	//10900
	IDC_BUTTON_CLEAR_MRU_FILE,		HIDC_BUTTON_CLEAR_MRU_FILE,			//�������N���A�i�t�@�C���j
	IDC_BUTTON_CLEAR_MRU_FOLDER,	HIDC_BUTTON_CLEAR_MRU_FOLDER,		//�������N���A�i�t�H���_�j
	IDC_CHECK_FREECARET,			HIDC_CHECK_FREECARET,				//�t���[�J�[�\��
//DEL	IDC_CHECK_INDENT,				HIDC_CHECK_INDENT,					//�����C���f���g �F�^�C�v�ʂֈړ�
//DEL	IDC_CHECK_INDENT_WSPACE,		HIDC_CHECK_INDENT_WSPACE,			//�S�p�󔒂��C���f���g �F�^�C�v�ʂֈړ�
	IDC_CHECK_USETRAYICON,			HIDC_CHECK_USETRAYICON,				//�^�X�N�g���C���g��
	IDC_CHECK_STAYTASKTRAY,			HIDC_CHECK_STAYTASKTRAY,			//�^�X�N�g���C�ɏ풓
	IDC_CHECK_REPEATEDSCROLLSMOOTH,	HIDC_CHECK_REPEATEDSCROLLSMOOTH,	//�������炩�ɂ���
	IDC_CHECK_CLOSEALLCONFIRM,		HIDC_CHECK_CLOSEALLCONFIRM,			//[���ׂĕ���]�ő��ɕҏW�p�̃E�B���h�E������Ίm�F����	// 2006.12.25 ryoji
	IDC_CHECK_EXITCONFIRM,			HIDC_CHECK_EXITCONFIRM,				//�I���̊m�F
	IDC_CHECK_STOPS_BOTH_ENDS_WHEN_SEARCH_WORD, HIDC_CHECK_STOPS_WORD, //�P��P�ʂňړ�����Ƃ��ɒP��̗��[�Ɏ~�܂�
	IDC_CHECK_STOPS_BOTH_ENDS_WHEN_SEARCH_PARAGRAPH, HIDC_CHECK_STOPS_PARAGRAPH, // �i���P�ʂňړ�����Ƃ��ɒi���̗��[�Ɏ~�܂�
	IDC_CHECK_NOMOVE_ACTIVATE_BY_MOUSE, HIDC_CHECK_NOMOVE_ACTIVATE_BY_MOUSE,	// �}�E�X�N���b�N�ŃA�N�e�B�u�ɂȂ����Ƃ��̓J�[�\�����N���b�N�ʒu�Ɉړ����Ȃ� 2007.10.08 genta
	IDC_HOTKEY_TRAYMENU,			HIDC_HOTKEY_TRAYMENU,				//���N���b�N���j���[�̃V���[�g�J�b�g�L�[
	IDC_EDIT_REPEATEDSCROLLLINENUM,	HIDC_EDIT_REPEATEDSCROLLLINENUM,	//�X�N���[���s��
	IDC_EDIT_MAX_MRU_FILE,			HIDC_EDIT_MAX_MRU_FILE,				//�t�@�C�������̍ő吔
	IDC_EDIT_MAX_MRU_FOLDER,		HIDC_EDIT_MAX_MRU_FOLDER,			//�t�H���_�����̍ő吔
	IDC_RADIO_CARETTYPE0,			HIDC_RADIO_CARETTYPE0,				//�J�[�\���`��iWindows���j
	IDC_RADIO_CARETTYPE1,			HIDC_RADIO_CARETTYPE1,				//�J�[�\���`��iMS-DOS���j
	IDC_SPIN_REPEATEDSCROLLLINENUM,	HIDC_EDIT_REPEATEDSCROLLLINENUM,
	IDC_SPIN_MAX_MRU_FILE,			HIDC_EDIT_MAX_MRU_FILE,
	IDC_SPIN_MAX_MRU_FOLDER,		HIDC_EDIT_MAX_MRU_FOLDER,
	IDC_CHECK_MEMDC,				HIDC_CHECK_MEMDC,					//��ʃL���b�V�����g��
	IDC_COMBO_WHEEL_PAGESCROLL,		HIDC_COMBO_WHEEL_PAGESCROLL,		// �g�ݍ��킹�ăz�C�[�����삵�����y�[�W�X�N���[������		// 2009.01.12 nasukoji
	IDC_COMBO_WHEEL_HSCROLL,		HIDC_COMBO_WHEEL_HSCROLL,			// �g�ݍ��킹�ăz�C�[�����삵�������X�N���[������			// 2009.01.12 nasukoji
//	IDC_STATIC,						-1,
	0, 0
};
//@@@ 2001.02.04 End

int	CPropCommon::SearchIntArr( int nKey, int* pnArr, int nArrNum )
{
	int i;
	for( i = 0; i < nArrNum; ++i ){
		if( nKey == pnArr[i] ){
			return i;
		}
	}
	return -1;
}

/*!
	@param hwndDlg �_�C�A���O�{�b�N�X��Window Handle
	@param uMsg ���b�Z�[�W
	@param wParam �p�����[�^1
	@param lParam �p�����[�^2
*/
INT_PTR CALLBACK CPropCommon::DlgProc_PROP_GENERAL(
	HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return DlgProc( &CPropCommon::DispatchEvent_p1, hwndDlg, uMsg, wParam, lParam );
}

/*!
	�v���p�e�B�y�[�W���Ƃ�Window Procedure�������Ɏ�邱�Ƃ�
	�����̋��ʉ���_�����D

	@param DispatchPage �^��Window Procedure�̃����o�֐��|�C���^
	@param hwndDlg �_�C�A���O�{�b�N�X��Window Handlw
	@param uMsg ���b�Z�[�W
	@param wParam �p�����[�^1
	@param lParam �p�����[�^2
*/
INT_PTR CPropCommon::DlgProc(
	INT_PTR (CPropCommon::*DispatchPage)( HWND, UINT, WPARAM, LPARAM ),
	HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam
)
{
	PROPSHEETPAGE*	pPsp;
	CPropCommon*	pCPropCommon;
	switch( uMsg ){
	case WM_INITDIALOG:
		pPsp = (PROPSHEETPAGE*)lParam;
		pCPropCommon = ( CPropCommon* )(pPsp->lParam);
		if( NULL != pCPropCommon ){
			return (pCPropCommon->*DispatchPage)( hwndDlg, uMsg, wParam, pPsp->lParam );
		}else{
			return FALSE;
		}
	default:
		// Modified by KEITA for WIN64 2003.9.6
		pCPropCommon = ( CPropCommon* )::GetWindowLongPtr( hwndDlg, DWLP_USER );
		if( NULL != pCPropCommon ){
			return (pCPropCommon->*DispatchPage)( hwndDlg, uMsg, wParam, lParam );
		}else{
			return FALSE;
		}
	}
}
//	To Here Jun. 2, 2001 genta

//	@date 2002.2.17 YAZAKI CShareData�̃C���X�^���X�́ACProcess�ɂЂƂ���̂݁B
CPropCommon::CPropCommon()
{
//	int		i;
//	long	lPathLen;

	/* ���L�f�[�^�\���̂̃A�h���X��Ԃ� */
	m_pShareData = CShareData::getInstance()->GetShareData();

	m_hInstance = NULL;		/* �A�v���P�[�V�����C���X�^���X�̃n���h�� */
	m_hwndParent = NULL;	/* �I�[�i�[�E�B���h�E�̃n���h�� */
	m_hwndThis  = NULL;		/* ���̃_�C�A���O�̃n���h�� */
	m_nPageNum = ID_PAGENUM_GENERAL;

	/* �w���v�t�@�C���̃t���p�X��Ԃ� */
	m_pszHelpFile = CEditApp::GetHelpFilePath();

	return;
}





CPropCommon::~CPropCommon()
{
}





/* ������ */
//@@@ 2002.01.03 YAZAKI m_tbMyButton�Ȃǂ�CShareData����CMenuDrawer�ֈړ��������Ƃɂ��C���B
void CPropCommon::Create( HINSTANCE hInstApp, HWND hwndParent, CImageListMgr* cIcons, CSMacroMgr* pMacro, CMenuDrawer* pMenuDrawer )
{
	m_hInstance = hInstApp;		/* �A�v���P�[�V�����C���X�^���X�̃n���h�� */
	m_hwndParent = hwndParent;	/* �I�[�i�[�E�B���h�E�̃n���h�� */
	m_pcIcons = cIcons;
	m_pcSMacro = pMacro;

	// 2007.11.02 ryoji �}�N���ݒ��ύX�������ƁA��ʂ���Ȃ��ŃJ�X�^�����j���[�A�c�[���o�[�A
	//                  �L�[���蓖�Ẳ�ʂɐ؂�ւ������Ɋe��ʂŃ}�N���ݒ�̕ύX�����f�����悤�A
	//                  m_MacroTable�i���[�J�������o�j��m_cLookup������������
	m_cLookup.Init( m_hInstance, m_MacroTable, &m_Common );	//	�@�\���E�ԍ�resolve�N���X�D

//@@@ 2002.01.03 YAZAKI m_tbMyButton�Ȃǂ�CShareData����CMenuDrawer�ֈړ��������Ƃɂ��C���B
	m_pcMenuDrawer = pMenuDrawer;

	return;
}





/* �F�I���_�C�A���O */
BOOL CPropCommon::SelectColor( HWND hwndParent, COLORREF* pColor )
{
	int			i;
	CHOOSECOLOR	cc;
	DWORD	dwCustColors[16] ;
	for( i = 0; i < 16; i++ ){
		dwCustColors[i] = (DWORD)RGB( 255, 255, 255 );
	}
	cc.lStructSize = sizeof( cc );
	cc.hwndOwner = hwndParent;
	cc.hInstance = NULL;
	cc.rgbResult = *pColor;
	cc.lpCustColors = (LPDWORD) dwCustColors;
	cc.Flags = /*CC_PREVENTFULLOPEN |*/ CC_RGBINIT;
	cc.lCustData = NULL;
	cc.lpfnHook = NULL;
	cc.lpTemplateName = NULL;
	if( FALSE == ::ChooseColor( &cc ) ){
		return FALSE;
	}
	*pColor = cc.rgbResult;
	return TRUE;
}


//	From Here Jun. 2, 2001 genta
/*!
	�u���ʐݒ�v�v���p�e�B�V�[�g�̍쐬���ɕK�v�ȏ���
	�ێ�����\����
*/
struct ComPropSheetInfo {
	const TCHAR* szTabname;									//!< TAB�̕\����
	unsigned int resId;										//!< Property sheet�ɑΉ�����Dialog resource
	INT_PTR (CALLBACK *DProc)(HWND, UINT, WPARAM, LPARAM);	//!< Dialog Procedure
};
//	To Here Jun. 2, 2001 genta

//	�L�[���[�h�F���ʐݒ�^�u����(�v���p�e�B�V�[�g)
/*! �v���p�e�B�V�[�g�̍쐬
	@date 2002.2.17 YAZAKI CShareData�̃C���X�^���X�́ACProcess�ɂЂƂ���̂݁B
*/
int CPropCommon::DoPropertySheet( int nPageNum/*, int nActiveItem*/ )
{
	int					nRet;
	int					nIdx;
	int					i;

	//	From Here Jun. 2, 2001 genta
	//	Feb. 11, 2007 genta URL��TAB�Ɠ��ꊷ��	// 2007.02.13 �����ύX�iTAB��WIN�̎��Ɂj
	//!	�u���ʐݒ�v�v���p�e�B�V�[�g�̍쐬���ɕK�v�ȏ��̔z��D
	//	�����ύX Win,Toolbar,Tab,Statusbar�̏��ɁAFile,FileName ����	2008/6/22 Uchi 
	static ComPropSheetInfo ComPropSheetInfoList[] = {
		{ _T("�S��"), 				IDD_PROP1P1,		DlgProc_PROP_GENERAL },
		{ _T("�E�B���h�E"),			IDD_PROP_WIN,		DlgProc_PROP_WIN },

		{ _T("�c�[���o�["),			IDD_PROP_TOOLBAR,	DlgProc_PROP_TOOLBAR },
		{ _T("�^�u�o�["),			IDD_PROP_TAB,		DlgProc_PROP_TAB },

		{ _T("�ҏW"),				IDD_PROP_EDIT,		DlgProc_PROP_EDIT },
		{ _T("�t�@�C��"),			IDD_PROP_FILE,		DlgProc_PROP_FILE },
		{ _T("�t�@�C�����\��"),		IDD_PROP_FNAME,		DlgProc_PROP_FILENAME},
		{ _T("�o�b�N�A�b�v"),		IDD_PROP_BACKUP,	DlgProc_PROP_BACKUP },
		{ _T("����"),				IDD_PROP_FORMAT,	DlgProc_PROP_FORMAT },
		{ _T("����"),				IDD_PROP_GREP,		DlgProc_PROP_GREP },	// 2006.08.23 ryoji �^�C�g���ύX�iGrep -> �����j
		{ _T("�L�[���蓖��"),		IDD_PROP_KEYBIND,	DlgProc_PROP_KEYBIND },
		{ _T("�J�X�^�����j���["),	IDD_PROP_CUSTMENU,	DlgProc_PROP_CUSTMENU },
		{ _T("�����L�[���[�h"),		IDD_PROP_KEYWORD,	DlgProc_PROP_KEYWORD },
		{ _T("�x��"),				IDD_PROP_HELPER,	DlgProc_PROP_HELPER },
		{ _T("�}�N��"),				IDD_PROP_MACRO,		DlgProc_PROP_MACRO },
	};

	PROPSHEETPAGE		psp[32];
	for( nIdx = 0, i = 0; i < _countof(ComPropSheetInfoList) && nIdx < 32 ; i++ ){
		if( ComPropSheetInfoList[i].szTabname != NULL ){
			PROPSHEETPAGE *p = &psp[nIdx];
			memset( p, 0, sizeof( *p ) );
			p->dwSize      = sizeof( *p );
			p->dwFlags     = PSP_USETITLE | PSP_HASHELP;
			p->hInstance   = m_hInstance;
			p->pszTemplate = MAKEINTRESOURCE( ComPropSheetInfoList[i].resId );
			p->pszIcon     = NULL;
			p->pfnDlgProc  = (DLGPROC)(ComPropSheetInfoList[i].DProc);
			p->pszTitle    = ComPropSheetInfoList[i].szTabname;
			p->lParam      = (LPARAM)this;
			p->pfnCallback = NULL;
			nIdx++;
		}
	}
	//	To Here Jun. 2, 2001 genta

	PROPSHEETHEADER		psh;
	memset( &psh, 0, sizeof( psh ) );
	
	//	Jun. 29, 2002 ������
	//	Windows 95�΍�DProperty Sheet�̃T�C�Y��Windows95���F���ł��镨�ɌŒ肷��D
	psh.dwSize = sizeof_old_PROPSHEETHEADER;

	//	JEPROtest Sept. 30, 2000 ���ʐݒ�̉B��[�K�p]�{�^���̐��̂͂����B�s���̃R�����g�A�E�g�����ւ��Ă݂�΂킩��
	psh.dwFlags    = PSH_NOAPPLYNOW | PSH_PROPSHEETPAGE;
	psh.hwndParent = m_hwndParent;
	psh.hInstance  = m_hInstance;
	psh.pszIcon    = NULL;
	psh.pszCaption = _T("���ʐݒ�");
	psh.nPages     = nIdx;

	//- 20020106 aroka # psh.nStartPage �� unsigned �Ȃ̂ŕ��ɂȂ�Ȃ�
	if( -1 == nPageNum ){
		psh.nStartPage = m_nPageNum;
	}else
	if( 0 > nPageNum ){			//- 20020106 aroka
		psh.nStartPage = 0;
	}else{
		psh.nStartPage = nPageNum;
	}
	if( psh.nPages - 1 < psh.nStartPage ){
		psh.nStartPage = psh.nPages - 1;
	}

	psh.ppsp = (LPCPROPSHEETPAGE)psp;
	psh.pfnCallback = NULL;

	nRet = MyPropertySheet( &psh );	// 2007.05.24 ryoji �Ǝ��g���v���p�e�B�V�[�g
	if( -1 == nRet ){
		TCHAR*	pszMsgBuf;
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			::GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// �f�t�H���g����
			(LPTSTR)&pszMsgBuf,
			0,
			NULL
		);
		PleaseReportToAuthor(
			NULL,
			_T("CPropCommon::DoPropertySheet()���ŃG���[���o�܂����B\n")
			_T("psh.nStartPage=[%d]\n")
			_T("::PropertySheet()���s\n")
			_T("\n")
			_T("%s\n"),
			psh.nStartPage,
			pszMsgBuf
		);
		::LocalFree( pszMsgBuf );
	}

	return nRet;
}

/*!	ShareData����ꎞ�̈�֐ݒ���R�s�[����
	@date 2002.12.11 Moca CEditDoc::OpenPropertySheet����ړ�
*/
void CPropCommon::InitData( void )
{
	int i;
	m_Common = m_pShareData->m_Common;
	m_nKeyNameArrNum = m_pShareData->m_nKeyNameArrNum;
	for( i = 0; i < _countof( m_pShareData->m_pKeyNameArr ); ++i ){
		m_pKeyNameArr[i] = m_pShareData->m_pKeyNameArr[i];
	}
	m_CKeyWordSetMgr = m_pShareData->m_CKeyWordSetMgr;

	//2002/04/25 YAZAKI STypeConfig�S�̂�ێ�����K�v�͂Ȃ��B
	for( i = 0; i < MAX_TYPES; ++i ){
		for( int j = 0; j < MAX_KEYWORDSET_PER_TYPE; j++ ){
			m_Types_nKeyWordSetIdx[i][j] = m_pShareData->m_Types[i].m_nKeyWordSetIdx[j];
		}
	}
	/* �}�N���֌W
	@@@ 2002.01.03 YAZAKI ���ʐݒ�w�}�N���x���^�u��؂�ւ��邾���Őݒ肪�ۑ�����Ȃ��悤�ɁB
	*/
	for( i = 0; i < MAX_CUSTMACRO; ++i ){
		m_MacroTable[i] = m_pShareData->m_MacroTable[i];
	}
	memcpy( m_szMACROFOLDER, m_pShareData->m_szMACROFOLDER, sizeof( m_pShareData->m_szMACROFOLDER ) );
	m_nMacroOnOpened = m_pShareData->m_nMacroOnOpened;	/* �I�[�v���㎩�����s�}�N���ԍ� */	//@@@ 2006.09.01 ryoji
	m_nMacroOnTypeChanged = m_pShareData->m_nMacroOnTypeChanged;	/* �^�C�v�ύX�㎩�����s�}�N���ԍ� */	//@@@ 2006.09.01 ryoji
	m_nMacroOnSave = m_pShareData->m_nMacroOnSave;	/* �ۑ��O�������s�}�N���ԍ� */	//@@@ 2006.09.01 ryoji

	// �t�@�C�����ȈՕ\���֌W
	memcpy( m_szTransformFileNameFrom, m_pShareData->m_szTransformFileNameFrom,
		sizeof( m_pShareData->m_szTransformFileNameFrom ) );
	memcpy( m_szTransformFileNameTo, m_pShareData->m_szTransformFileNameTo,
		sizeof( m_pShareData->m_szTransformFileNameTo ) );
	m_nTransformFileNameArrNum = m_pShareData->m_nTransformFileNameArrNum;

}

/*!	ShareData �� �ݒ��K�p�E�R�s�[����
	@note ShareData�ɃR�s�[���邾���Ȃ̂ŁC�X�V�v���Ȃǂ́C���p���鑤�ŏ������Ă��炤
	@date 2002.12.11 Moca CEditDoc::OpenPropertySheet����ړ�
*/
void CPropCommon::ApplyData( void )
{
	int i;

	for( i = 0; i < sizeof( m_pShareData->m_pKeyNameArr ) / sizeof( m_pShareData->m_pKeyNameArr[0] ); ++i ){
		m_pShareData->m_pKeyNameArr[i] = m_pKeyNameArr[i];
	}
	m_pShareData->m_CKeyWordSetMgr = m_CKeyWordSetMgr;

	m_pShareData->m_Common = m_Common;

	for( i = 0; i < MAX_TYPES; ++i ){
		//2002/04/25 YAZAKI STypeConfig�S�̂�ێ�����K�v�͂Ȃ��B
		/* �ύX���ꂽ�ݒ�l�̃R�s�[ */
		for( int j = 0; j < MAX_KEYWORDSET_PER_TYPE; j++ ){
			m_pShareData->m_Types[i].m_nKeyWordSetIdx[j] = m_Types_nKeyWordSetIdx[i][j];
		}
	}

	/* �}�N���֌W */
	for( i = 0; i < MAX_CUSTMACRO; ++i ){
		m_pShareData->m_MacroTable[i] = m_MacroTable[i];
	}
	memcpy( m_pShareData->m_szMACROFOLDER, m_szMACROFOLDER, sizeof( m_pShareData->m_szMACROFOLDER ) );
	m_pShareData->m_nMacroOnOpened = m_nMacroOnOpened;	/* �I�[�v���㎩�����s�}�N���ԍ� */	//@@@ 2006.09.01 ryoji
	m_pShareData->m_nMacroOnTypeChanged = m_nMacroOnTypeChanged;	/* �^�C�v�ύX�㎩�����s�}�N���ԍ� */	//@@@ 2006.09.01 ryoji
	m_pShareData->m_nMacroOnSave = m_nMacroOnSave;	/* �ۑ��O�������s�}�N���ԍ� */	//@@@ 2006.09.01 ryoji

	// �t�@�C�����ȈՕ\���֌W
	// �O�̂��߁C����������O�� 0 ��ݒ肵�Ă���
	m_pShareData->m_nTransformFileNameArrNum = 0;
	memcpy( m_pShareData->m_szTransformFileNameFrom, m_szTransformFileNameFrom,
		sizeof( m_pShareData->m_szTransformFileNameFrom ) );
	memcpy( m_pShareData->m_szTransformFileNameTo, m_szTransformFileNameTo,
		sizeof( m_pShareData->m_szTransformFileNameTo ) );

	m_pShareData->m_nTransformFileNameArrNum = m_nTransformFileNameArrNum;

}




/* p1 ���b�Z�[�W���� */
INT_PTR CPropCommon::DispatchEvent_p1(
	HWND	hwndDlg,	// handle to dialog box
	UINT	uMsg,		// message
	WPARAM	wParam,		// first message parameter
	LPARAM	lParam 		// second message parameter
)
{
	WORD		wNotifyCode;
	WORD		wID;
//	HWND		hwndCtl;
	NMHDR*		pNMHDR;
	NM_UPDOWN*	pMNUD;
	int			idCtrl;
	int			nVal;
//	LPDRAWITEMSTRUCT pDis;

	switch( uMsg ){

	case WM_INITDIALOG:
		/* �_�C�A���O�f�[�^�̐ݒ� p1 */
		SetData_p1( hwndDlg );
		// Modified by KEITA for WIN64 2003.9.6
		::SetWindowLongPtr( hwndDlg, DWLP_USER, lParam );

		/* ���[�U�[���G�f�B�b�g �R���g���[���ɓ��͂ł���e�L�X�g�̒����𐧌����� */

		return TRUE;
	case WM_COMMAND:
		wNotifyCode	= HIWORD(wParam);	/* �ʒm�R�[�h */
		wID			= LOWORD(wParam);	/* ����ID� �R���g���[��ID� �܂��̓A�N�Z�����[�^ID */
//		hwndCtl		= (HWND) lParam;	/* �R���g���[���̃n���h�� */
		switch( wNotifyCode ){
		/* �{�^���^�`�F�b�N�{�b�N�X���N���b�N���ꂽ */
		case BN_CLICKED:
			switch( wID ){

			case IDC_CHECK_USETRAYICON:	/* �^�X�N�g���C���g�� */
			// From Here 2001.12.03 hor
			//		���삵�ɂ������ĕ]���������̂Ń^�X�N�g���C�֌W��Enable�������߂܂���
			//@@@ YAZAKI 2001.12.31 IDC_CHECKSTAYTASKTRAY�̃A�N�e�B�u�A��A�N�e�B�u�̂ݐ���B
				if( ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_USETRAYICON ) ){
					::EnableWindow( ::GetDlgItem( hwndDlg, IDC_CHECK_STAYTASKTRAY ), TRUE );
				}else{
					::EnableWindow( ::GetDlgItem( hwndDlg, IDC_CHECK_STAYTASKTRAY ), FALSE );
				}
			// To Here 2001.12.03 hor
				return TRUE;

			case IDC_CHECK_STAYTASKTRAY:	/* �^�X�N�g���C�ɏ풓 */
				return TRUE;

			case IDC_BUTTON_CLEAR_MRU_FILE:
				/* �t�@�C���̗������N���A */
				if( IDCANCEL == ::MYMESSAGEBOX( hwndDlg, MB_OKCANCEL | MB_ICONQUESTION, GSTR_APPNAME,
					_T("�ŋߎg�����t�@�C���̗������폜���܂��B\n��낵���ł����H\n") ) ){
					return TRUE;
				}
//@@@ 2001.12.26 YAZAKI MRU���X�g�́ACMRU�Ɉ˗�����
//				m_pShareData->m_nMRUArrNum = 0;
				{
					CMRUFile cMRU;
					cMRU.ClearAll();
				}
				::MYMESSAGEBOX( hwndDlg, MB_OK | MB_ICONINFORMATION, GSTR_APPNAME, _T("�ŋߎg�����t�@�C���̗������폜���܂����B\n") );
				return TRUE;
			case IDC_BUTTON_CLEAR_MRU_FOLDER:
				/* �t�H���_�̗������N���A */
				if( IDCANCEL == ::MYMESSAGEBOX( hwndDlg, MB_OKCANCEL | MB_ICONQUESTION, GSTR_APPNAME,
					_T("�ŋߎg�����t�H���_�̗������폜���܂��B\n��낵���ł����H\n") ) ){
					return TRUE;
				}
//@@@ 2001.12.26 YAZAKI OPENFOLDER���X�g�́ACMRUFolder�ɂ��ׂĈ˗�����
//				m_pShareData->m_nOPENFOLDERArrNum = 0;
				{
					CMRUFolder cMRUFolder;	//	MRU���X�g�̏������B���x�������Ɩ�肠��H
					cMRUFolder.ClearAll();
				}
				::MYMESSAGEBOX( hwndDlg, MB_OK | MB_ICONINFORMATION, GSTR_APPNAME, _T("�ŋߎg�����t�H���_�̗������폜���܂����B\n") );
				return TRUE;

			}
			break;	/* BN_CLICKED */
		// 2009.01.12 nasukoji	�R���{�{�b�N�X�̃��X�g�̍��ڂ��I�����ꂽ
		case CBN_SELENDOK:
			HWND	hwndCombo;
			int		nSelPos;

			switch( wID ){
			// �g�ݍ��킹�ăz�C�[�����삵�����y�[�W�X�N���[������
			case IDC_COMBO_WHEEL_PAGESCROLL:
				hwndCombo = ::GetDlgItem( hwndDlg, IDC_COMBO_WHEEL_PAGESCROLL );
				nSelPos = ::SendMessage( hwndCombo, CB_GETCURSEL, 0, 0 );
				hwndCombo = ::GetDlgItem( hwndDlg, IDC_COMBO_WHEEL_HSCROLL );
				if( nSelPos && nSelPos == ::SendMessage( hwndCombo, CB_GETCURSEL, 0, 0 ) ){
					::SendMessage( hwndCombo, CB_SETCURSEL, 0, 0 );
				}
				return TRUE;
			// �g�ݍ��킹�ăz�C�[�����삵�������X�N���[������
			case IDC_COMBO_WHEEL_HSCROLL:
				hwndCombo = ::GetDlgItem( hwndDlg, IDC_COMBO_WHEEL_HSCROLL );
				nSelPos = ::SendMessage( hwndCombo, CB_GETCURSEL, 0, 0 );
				hwndCombo = ::GetDlgItem( hwndDlg, IDC_COMBO_WHEEL_PAGESCROLL );
				if( nSelPos && nSelPos == ::SendMessage( hwndCombo, CB_GETCURSEL, 0, 0 ) ){
					::SendMessage( hwndCombo, CB_SETCURSEL, 0, 0 );
				}
				return TRUE;
			}
			break;	// CBN_SELENDOK
		}
		break;	/* WM_COMMAND */
	case WM_NOTIFY:
		idCtrl = (int)wParam;
		pNMHDR = (NMHDR*)lParam;
		pMNUD  = (NM_UPDOWN*)lParam;
		switch( idCtrl ){
		case IDC_SPIN_REPEATEDSCROLLLINENUM:
			/* �L�[���s�[�g���̃X�N���[���s�� */
//			MYTRACE_A( "IDC_SPIN_REPEATEDSCROLLLINENUM\n" );
			nVal = ::GetDlgItemInt( hwndDlg, IDC_EDIT_REPEATEDSCROLLLINENUM, NULL, FALSE );
			if( pMNUD->iDelta < 0 ){
				++nVal;
			}else
			if( pMNUD->iDelta > 0 ){
				--nVal;
			}
			if( nVal < 1 ){
				nVal = 1;
			}
			if( nVal > 10 ){
				nVal = 10;
			}
			::SetDlgItemInt( hwndDlg, IDC_EDIT_REPEATEDSCROLLLINENUM, nVal, FALSE );
			return TRUE;
		case IDC_SPIN_MAX_MRU_FILE:
			/* �t�@�C���̗���MAX */
//			MYTRACE_A( "IDC_SPIN_MAX_MRU_FILE\n" );
			nVal = ::GetDlgItemInt( hwndDlg, IDC_EDIT_MAX_MRU_FILE, NULL, FALSE );
			if( pMNUD->iDelta < 0 ){
				++nVal;
			}else
			if( pMNUD->iDelta > 0 ){
				--nVal;
			}
			if( nVal < 0 ){
				nVal = 0;
			}
			if( nVal > MAX_MRU ){
				nVal = MAX_MRU;
			}
			::SetDlgItemInt( hwndDlg, IDC_EDIT_MAX_MRU_FILE, nVal, FALSE );
			return TRUE;
		case IDC_SPIN_MAX_MRU_FOLDER:
			/* �t�H���_�̗���MAX */
//			MYTRACE_A( "IDC_SPIN_MAX_MRU_FOLDER\n" );
			nVal = ::GetDlgItemInt( hwndDlg, IDC_EDIT_MAX_MRU_FOLDER, NULL, FALSE );
			if( pMNUD->iDelta < 0 ){
				++nVal;
			}else
			if( pMNUD->iDelta > 0 ){
				--nVal;
			}
			if( nVal < 0 ){
				nVal = 0;
			}
			if( nVal > MAX_OPENFOLDER ){
				nVal = MAX_OPENFOLDER;
			}
			::SetDlgItemInt( hwndDlg, IDC_EDIT_MAX_MRU_FOLDER, nVal, FALSE );
			return TRUE;
		default:
			switch( pNMHDR->code ){
			case PSN_HELP:
				OnHelp( hwndDlg, IDD_PROP1P1 );
				return TRUE;
			case PSN_KILLACTIVE:
//				MYTRACE_A( "p1 PSN_KILLACTIVE\n" );
				/* �_�C�A���O�f�[�^�̎擾 p1 */
				GetData_p1( hwndDlg );
				return TRUE;
//@@@ 2002.01.03 YAZAKI �Ō�ɕ\�����Ă����V�[�g�𐳂����o���Ă��Ȃ��o�O�C��
			case PSN_SETACTIVE:
				m_nPageNum = ID_PAGENUM_GENERAL;	//Oct. 25, 2000 JEPRO ZENPAN1��ZENPAN �ɕύX(�Q�Ƃ��Ă���̂�CPropCommon.cpp�݂̂�1�ӏ�)
				return TRUE;
			}
			break;
		}

//		MYTRACE_A( "pNMHDR->hwndFrom=%xh\n", pNMHDR->hwndFrom );
//		MYTRACE_A( "pNMHDR->idFrom  =%xh\n", pNMHDR->idFrom );
//		MYTRACE_A( "pNMHDR->code    =%xh\n", pNMHDR->code );
//		MYTRACE_A( "pMNUD->iPos    =%d\n", pMNUD->iPos );
//		MYTRACE_A( "pMNUD->iDelta  =%d\n", pMNUD->iDelta );
		break;

//@@@ 2001.02.04 Start by MIK: Popup Help
	case WM_HELP:
		{
			HELPINFO *p = (HELPINFO *)lParam;
			MyWinHelp( (HWND)p->hItemHandle, m_pszHelpFile, HELP_WM_HELP, (ULONG_PTR)(LPVOID)p_helpids );	// 2006.10.10 ryoji MyWinHelp�ɕύX�ɕύX
		}
		return TRUE;
		/*NOTREACHED*/
//		break;
//@@@ 2001.02.04 End

//@@@ 2001.12.22 Start by MIK: Context Menu Help
	//Context Menu
	case WM_CONTEXTMENU:
		MyWinHelp( hwndDlg, m_pszHelpFile, HELP_CONTEXTMENU, (ULONG_PTR)(LPVOID)p_helpids );	// 2006.10.10 ryoji MyWinHelp�ɕύX�ɕύX
		return TRUE;
//@@@ 2001.12.22 End

	}
	return FALSE;
}





/* �_�C�A���O�f�[�^�̐ݒ� p1 */
void CPropCommon::SetData_p1( HWND hwndDlg )
{
	BOOL	bRet;

	/* �J�[�\���̃^�C�v 0=win 1=dos  */
	if( 0 == m_Common.GetCaretType() ){
		::CheckDlgButton( hwndDlg, IDC_RADIO_CARETTYPE0, TRUE );
		::CheckDlgButton( hwndDlg, IDC_RADIO_CARETTYPE1, FALSE );
	}else{
		::CheckDlgButton( hwndDlg, IDC_RADIO_CARETTYPE0, FALSE );
		::CheckDlgButton( hwndDlg, IDC_RADIO_CARETTYPE1, TRUE );
	}


	/* �t���[�J�[�\�����[�h */
	::CheckDlgButton( hwndDlg, IDC_CHECK_FREECARET, m_Common.m_bIsFreeCursorMode );

	/* �P��P�ʂňړ�����Ƃ��ɁA�P��̗��[�Ŏ~�܂邩 */
	::CheckDlgButton( hwndDlg, IDC_CHECK_STOPS_BOTH_ENDS_WHEN_SEARCH_WORD, m_Common.m_bStopsBothEndsWhenSearchWord );

	/* �i���P�ʂňړ�����Ƃ��ɁA�i���̗��[�Ŏ~�܂邩 */
	::CheckDlgButton( hwndDlg, IDC_CHECK_STOPS_BOTH_ENDS_WHEN_SEARCH_PARAGRAPH, m_Common.m_bStopsBothEndsWhenSearchParagraph );

	//	2007.10.08 genta �}�E�X�N���b�N�ŃA�N�e�B�u�ɂȂ����Ƃ��̓J�[�\�����N���b�N�ʒu�Ɉړ����Ȃ� (2007.10.02 by nasukoji)
	::CheckDlgButton( hwndDlg, IDC_CHECK_NOMOVE_ACTIVATE_BY_MOUSE, m_Common.m_bNoCaretMoveByActivation );

	/* [���ׂĕ���]�ő��ɕҏW�p�̃E�B���h�E������Ίm�F���� */	// 2006.12.25 ryoji
	::CheckDlgButton( hwndDlg, IDC_CHECK_CLOSEALLCONFIRM, m_Common.m_bCloseAllConfirm );

	/* �I�����̊m�F������ */
	::CheckDlgButton( hwndDlg, IDC_CHECK_EXITCONFIRM, m_Common.m_bExitConfirm );

	/* �L�[���s�[�g���̃X�N���[���s�� */
	bRet = ::SetDlgItemInt( hwndDlg, IDC_EDIT_REPEATEDSCROLLLINENUM, m_Common.m_nRepeatedScrollLineNum, FALSE );

	/* �L�[���s�[�g���̃X�N���[�������炩�ɂ��邩 */
	::CheckDlgButton( hwndDlg, IDC_CHECK_REPEATEDSCROLLSMOOTH, m_Common.m_nRepeatedScroll_Smooth );

	// 2009.01.12 nasukoji	�g�ݍ��킹�ăz�C�[�����삵�����y�[�W�X�N���[������
	HWND	hwndCombo;
	int		nSelPos;
	int		i;

	hwndCombo = ::GetDlgItem( hwndDlg, IDC_COMBO_WHEEL_PAGESCROLL );
	::SendMessage( hwndCombo, CB_RESETCONTENT, 0, 0 );
	nSelPos = 0;
	for( i = 0; i < _countof( SpecialScrollModeArr ); ++i ){
		::SendMessage( hwndCombo, CB_INSERTSTRING, i, (LPARAM)SpecialScrollModeArr[i].pszName );
		if( SpecialScrollModeArr[i].nMethod == m_Common.m_nPageScrollByWheel ){	// �y�[�W�X�N���[���Ƃ���g�ݍ��킹����
			nSelPos = i;
		}
	}
	::SendMessage( hwndCombo, CB_SETCURSEL, nSelPos, 0 );

	// 2009.01.12 nasukoji	�g�ݍ��킹�ăz�C�[�����삵�������X�N���[������
	hwndCombo = ::GetDlgItem( hwndDlg, IDC_COMBO_WHEEL_HSCROLL );
	::SendMessage( hwndCombo, CB_RESETCONTENT, 0, 0 );
	nSelPos = 0;
	for( i = 0; i < _countof( SpecialScrollModeArr ); ++i ){
		::SendMessage( hwndCombo, CB_INSERTSTRING, i, (LPARAM)SpecialScrollModeArr[i].pszName );
		if( SpecialScrollModeArr[i].nMethod == m_Common.m_nHorizontalScrollByWheel ){	// ���X�N���[���Ƃ���g�ݍ��킹����
			nSelPos = i;
		}
	}
	::SendMessage( hwndCombo, CB_SETCURSEL, nSelPos, 0 );

	// 2007.09.09 Moca ��ʃL���b�V���ݒ�ǉ�
	// ��ʃL���b�V�����g��
	::CheckDlgButton( hwndDlg, IDC_CHECK_MEMDC, m_Common.m_bUseCompotibleBMP );

	/* �t�@�C���̗���MAX */
	bRet = ::SetDlgItemInt( hwndDlg, IDC_EDIT_MAX_MRU_FILE, m_Common.m_nMRUArrNum_MAX, FALSE );

	/* �t�H���_�̗���MAX */
	bRet = ::SetDlgItemInt( hwndDlg, IDC_EDIT_MAX_MRU_FOLDER, m_Common.m_nOPENFOLDERArrNum_MAX, FALSE );

	/* �^�X�N�g���C���g�� */
	::CheckDlgButton( hwndDlg, IDC_CHECK_USETRAYICON, m_Common.m_bUseTaskTray );
// From Here 2001.12.03 hor
//@@@ YAZAKI 2001.12.31 �����͐��䂷��B
	if( m_Common.m_bUseTaskTray ){
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_CHECK_STAYTASKTRAY ), TRUE );
	}else{
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_CHECK_STAYTASKTRAY ), FALSE );
	}
// To Here 2001.12.03 hor
	/* �^�X�N�g���C�ɏ풓 */
	::CheckDlgButton( hwndDlg, IDC_CHECK_STAYTASKTRAY, m_Common.m_bStayTaskTray );

	/* �^�X�N�g���C���N���b�N���j���[�̃V���[�g�J�b�g */
	::SendMessage( ::GetDlgItem( hwndDlg, IDC_HOTKEY_TRAYMENU ), HKM_SETHOTKEY, MAKEWORD( m_Common.m_wTrayMenuHotKeyCode, m_Common.m_wTrayMenuHotKeyMods ), 0 );

	return;
}





/* �_�C�A���O�f�[�^�̎擾 p1 */
int CPropCommon::GetData_p1( HWND hwndDlg )
{
	/* �J�[�\���̃^�C�v 0=win 1=dos  */
	if( ::IsDlgButtonChecked( hwndDlg, IDC_RADIO_CARETTYPE0 ) ){
		m_Common.SetCaretType(0);
	}
	if( ::IsDlgButtonChecked( hwndDlg, IDC_RADIO_CARETTYPE1 ) ){
		m_Common.SetCaretType(1);
	}

	/* �t���[�J�[�\�����[�h */
	m_Common.m_bIsFreeCursorMode = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_FREECARET );

	/* �P��P�ʂňړ�����Ƃ��ɁA�P��̗��[�Ŏ~�܂邩 */
	m_Common.m_bStopsBothEndsWhenSearchWord = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_STOPS_BOTH_ENDS_WHEN_SEARCH_WORD );
	//	2007.10.08 genta �}�E�X�N���b�N�ŃA�N�e�B�u�ɂȂ����Ƃ��̓J�[�\�����N���b�N�ʒu�Ɉړ����Ȃ� (2007.10.02 by nasukoji)
	m_Common.m_bNoCaretMoveByActivation = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_NOMOVE_ACTIVATE_BY_MOUSE );

	/* �i���P�ʂňړ�����Ƃ��ɁA�i���̗��[�Ŏ~�܂邩 */
	m_Common.m_bStopsBothEndsWhenSearchParagraph = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_STOPS_BOTH_ENDS_WHEN_SEARCH_PARAGRAPH );

	/* [���ׂĕ���]�ő��ɕҏW�p�̃E�B���h�E������Ίm�F���� */	// 2006.12.25 ryoji
	m_Common.m_bCloseAllConfirm = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_CLOSEALLCONFIRM );

	/* �I�����̊m�F������ */
	m_Common.m_bExitConfirm = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_EXITCONFIRM );

	/* �L�[���s�[�g���̃X�N���[���s�� */
	m_Common.m_nRepeatedScrollLineNum = ::GetDlgItemInt( hwndDlg, IDC_EDIT_REPEATEDSCROLLLINENUM, NULL, FALSE );
	if( m_Common.m_nRepeatedScrollLineNum < 1 ){
		m_Common.m_nRepeatedScrollLineNum = 1;
	}
	if( m_Common.m_nRepeatedScrollLineNum > 10 ){
		m_Common.m_nRepeatedScrollLineNum = 10;
	}

	/* �L�[���s�[�g���̃X�N���[�������炩�ɂ��邩 */
	m_Common.m_nRepeatedScroll_Smooth = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_REPEATEDSCROLLSMOOTH );

	// 2009.01.12 nasukoji	�g�ݍ��킹�ăz�C�[�����삵�����y�[�W�X�N���[������
	HWND	hwndCombo;
	int		nSelPos;

	// 2007.09.09 Moca ��ʃL���b�V���ݒ�ǉ�
	// ��ʃL���b�V�����g��
	m_Common.m_bUseCompotibleBMP = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_MEMDC );

	hwndCombo = ::GetDlgItem( hwndDlg, IDC_COMBO_WHEEL_PAGESCROLL );
	nSelPos = ::SendMessage( hwndCombo, CB_GETCURSEL, 0, 0 );
	m_Common.m_nPageScrollByWheel = SpecialScrollModeArr[nSelPos].nMethod;			// �y�[�W�X�N���[���Ƃ���g�ݍ��킹����

	// 2009.01.12 nasukoji	�g�ݍ��킹�ăz�C�[�����삵�������X�N���[������
	hwndCombo = ::GetDlgItem( hwndDlg, IDC_COMBO_WHEEL_HSCROLL );
	nSelPos = ::SendMessage( hwndCombo, CB_GETCURSEL, 0, 0 );
	m_Common.m_nHorizontalScrollByWheel = SpecialScrollModeArr[nSelPos].nMethod;	// ���X�N���[���Ƃ���g�ݍ��킹����

	/* �t�@�C���̗���MAX */
	m_Common.m_nMRUArrNum_MAX = ::GetDlgItemInt( hwndDlg, IDC_EDIT_MAX_MRU_FILE, NULL, FALSE );
	if( m_Common.m_nMRUArrNum_MAX < 0 ){
		m_Common.m_nMRUArrNum_MAX = 0;
	}
	if( m_Common.m_nMRUArrNum_MAX > MAX_MRU ){
		m_Common.m_nMRUArrNum_MAX = MAX_MRU;
	}

	{	//�����̊Ǘ�	//@@@ 2003.04.09 MIK
		CRecent	cRecentFile;
		cRecentFile.EasyCreate( RECENT_FOR_FILE );
		cRecentFile.UpdateView();
		cRecentFile.Terminate();
	}

	/* �t�H���_�̗���MAX */
	m_Common.m_nOPENFOLDERArrNum_MAX = ::GetDlgItemInt( hwndDlg, IDC_EDIT_MAX_MRU_FOLDER, NULL, FALSE );
	if( m_Common.m_nOPENFOLDERArrNum_MAX < 0 ){
		m_Common.m_nOPENFOLDERArrNum_MAX = 0;
	}
	if( m_Common.m_nOPENFOLDERArrNum_MAX > MAX_OPENFOLDER ){
		m_Common.m_nOPENFOLDERArrNum_MAX = MAX_OPENFOLDER;
	}

	{	//�����̊Ǘ�	//@@@ 2003.04.09 MIK
		CRecent	cRecentFolder;
		cRecentFolder.EasyCreate( RECENT_FOR_FOLDER );
		cRecentFolder.UpdateView();
		cRecentFolder.Terminate();
	}

	/* �^�X�N�g���C���g�� */
	m_Common.m_bUseTaskTray = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_USETRAYICON );
//@@@ YAZAKI 2001.12.31 m_bUseTaskTray�Ɉ����Â���悤�ɁB
	if( m_Common.m_bUseTaskTray ){
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_CHECK_STAYTASKTRAY ), TRUE );
	}else{
		::EnableWindow( ::GetDlgItem( hwndDlg, IDC_CHECK_STAYTASKTRAY ), FALSE );
	}
	/* �^�X�N�g���C�ɏ풓 */
	m_Common.m_bStayTaskTray = ::IsDlgButtonChecked( hwndDlg, IDC_CHECK_STAYTASKTRAY );

	/* �^�X�N�g���C���N���b�N���j���[�̃V���[�g�J�b�g */
	LRESULT	lResult;
	lResult = ::SendMessage( ::GetDlgItem( hwndDlg, IDC_HOTKEY_TRAYMENU ), HKM_GETHOTKEY, 0, 0 );
	m_Common.m_wTrayMenuHotKeyCode = LOBYTE( lResult );
	m_Common.m_wTrayMenuHotKeyMods = HIBYTE( lResult );

	return TRUE;
}



/* �w���v */
//Stonee, 2001/05/18 �@�\�ԍ�����w���v�g�s�b�N�ԍ��𒲂ׂ�悤�ɂ���
void CPropCommon::OnHelp( HWND hwndParent, int nPageID )
{
	int		nContextID;
	switch( nPageID ){
	case IDD_PROP1P1:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_GENERAL);
		break;
	case IDD_PROP_FORMAT:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_FORMAT);
		break;
	case IDD_PROP_FILE:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_FILE);
		break;
//	Sept. 10, 2000 JEPRO ID�������ۂ̖��O�ɕύX���邽�߈ȉ��̍s�̓R�����g�A�E�g
//	�ύX�͏�����̍s(Sept. 9, 2000)�ōs���Ă���
//	case IDD_PROP1P5:
//		nContextID = 84;
//		break;
	case IDD_PROP_TOOLBAR:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_TOOLBAR);
		break;
	case IDD_PROP_KEYWORD:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_KEYWORD);
		break;
	case IDD_PROP_CUSTMENU:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_CUSTMENU);
		break;
	case IDD_PROP_HELPER:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_HELPER);
		break;

	// From Here Sept. 9, 2000 JEPRO ���ʐݒ�̃w���v�{�^���������Ȃ��Ȃ��Ă����������ȉ��̒ǉ��ɂ���ďC��
	case IDD_PROP_EDIT:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_EDIT);
		break;
	case IDD_PROP_BACKUP:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_BACKUP);
		break;
	case IDD_PROP_WIN:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_WINDOW);
		break;
	case IDD_PROP_TAB:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_TAB);
		break;
	case IDD_PROP_GREP:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_GREP);
		break;
	case IDD_PROP_KEYBIND:
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_KEYBIND);
		break;
	// To Here Sept. 9, 2000
	case IDD_PROP_MACRO:	//@@@ 2002.01.02
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_MACRO);
		break;
	case IDD_PROP_FNAME:	// 2002.12.09 Moca FNAME�ǉ�
		nContextID = ::FuncID_To_HelpContextID(F_OPTION_FNAME);
		break;

	default:
		nContextID = -1;
		break;
	}
	if( -1 != nContextID ){
		MyWinHelp( hwndParent, m_pszHelpFile, HELP_CONTEXT, nContextID );	// 2006.10.10 ryoji MyWinHelp�ɕύX�ɕύX
	}
	return;
}


/*[EOF]*/