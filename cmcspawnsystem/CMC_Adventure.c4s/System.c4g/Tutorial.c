/*-- Tutorial Helpers --*/

#strict 2

static const SHOWCTRLPOS_Top = 0,
              SHOWCTRLPOS_TopLeft = 3,
              SHOWCTRLPOS_TopRight = 4,
              SHOWCTRLPOS_BottomLeft = 2,
              SHOWCTRLPOS_BottomRight = 1;

static g_msgpos, g_msgoffx, g_msgoffy, g_msgwdt;

global func HasSpeech(string strMessage)
{
	var len = GetLength(strMessage);
	for (var i = 0; i < len; i++)
		if (GetChar(strMessage, i) == GetChar("$"))
			return true;
	return false;
}

global func TutorialMessage(string strMessage, int iPlrPlusOne)
{
	if(iPlrPlusOne)
	{
		iPlrPlusOne--;
		if(HasSpeech(strMessage))
			PlayerMessage(iPlrPlusOne, strMessage);
		if (GetLength(strMessage)) strMessage = Format("@%s", strMessage);
			CustomMessage(strMessage, 0, iPlrPlusOne, g_msgoffx, g_msgoffy, 0xFFFFFF, DECO, "Portrait:PCMK::0000FF::1", g_msgpos | MSG_DropSpeech, g_msgwdt);
	
		return true;
	}

	// Message with speech marker
	for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
	{
		if(HasSpeech(strMessage))
			PlayerMessage(GetPlayerByIndex(i, C4PT_User), strMessage);
		// Normal message display, in addition to speech output
		if (GetLength(strMessage)) strMessage = Format("@%s", strMessage);
			CustomMessage(strMessage, 0, i, g_msgoffx, g_msgoffy, 0xFFFFFF, DECO, "Portrait:PCMK::0000FF::1", g_msgpos | MSG_DropSpeech, g_msgwdt);
	}
	
	return true;
}

global func wait(int iTicks, bool fFake)
{
	if(fFake)
	{
		FakeScriptGo(false, g_TutorialObject);
		Schedule(Format("FakeScriptGo(true, Object(%d))", ObjectNumber(g_TutorialObject)), iTicks * 10);
		return true;
	}
	
	ScriptGo(0);
	Schedule("ScriptGo(1)", iTicks * 10);
	return true;
}

global func repeat()
{
	goto(ScriptCounter() - 1);
}

global func SetTutorialMessagePos(int posflags, int offx, int offy, int wdt)
{
	g_msgpos = posflags;
	g_msgoffx = offx;
	g_msgoffy = offy;
	g_msgwdt = wdt;
	return true;
}

global func GainTutorialAccess(int iKey)
{	
	if(!Inside(iKey, 1, 31))
		return false;
	
	for(var i = 0; i < GetPlayerCount(); i++)
	{
		var iData = GetPlrExtraData(i, "CMC_TutorialKeys");
		if(!(iData >> iKey & 1))
			SetPlrExtraData(i, "CMC_TutorialKeys", iData ^ 1 << iKey);
	}
	
	return true; 
}

global func HasTutorialAccess(int iKey, int iPlr)
{
	return GetPlrExtraData(iPlr, "CMC_TutorialKeys") >> iKey & 1;
}












