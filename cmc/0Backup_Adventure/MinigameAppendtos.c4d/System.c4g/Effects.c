/*-- Neues Script --*/

#strict 2

global func Indestructible(object pTarget)
{
	if(!pTarget && !(pTarget = this))
		return false;
	
	if(GetEffect("CMCIndestrucitble", pTarget))
		return false;
	
	return AddEffect("CMCIndestructible", pTarget, 101, 3);
}

global func FxCMCIndestructibleStart(object pTarget, int iNr)
{
	EffectVar(0, pTarget, iNr) = g_TutorialObject;
	return true;
}

global func FxCMCIndestructibleTimer(object pTarget, int iNr)
{
	if(EffectVar(0, pTarget, iNr) != g_TutorialObject) // Bei Mapchange löschen.
		return -1;
}

global func FxCMCIndestructibleDamage() { return 0; }

////////////////////////////////////////////////////////////////////////////////

// Lässt Spieler iForPlr warten. iForPlr = -1 -> Alle Spieler warten.
global func Wait(int iTime, int iForPlr, bool fMessage, string szCall)
{
	if(iForPlr == -1)
	{
		for(var i = 0; i < GetPlayerCount(); i++)
		{
			var plr = GetPlayerByIndex(i);
			for(var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Owner(plr)))
			{
				if(GetEffect("CMCWait", obj))
					RemoveEffect("CMCWait", obj);
				
				AddEffect("CMCWait", obj, 101, 1, 0, 0, iTime, fMessage, szCall);
				szCall = 0;
			}
		}
	}
	else
	{
		for(var obj in FindObjects(Find_OCF(OCF_CrewMember), Find_Owner(iForPlr)))
		{
			if(GetEffect("CMCWait", obj))
				RemoveEffect("CMCWait", obj);
				
			AddEffect("CMCWait", obj, 101, 1, 0, 0, iTime, fMessage, szCall);
			szCall = 0;
		}
	}
	return true;
}

global func FxCMCWaitStart(object pTarget, int iNr, temp, int iTime, bool fMessage, string szCall)
{
	if(temp)
		return -1;

	EffectVar(0, pTarget, iNr) = g_TutorialObject;
	EffectVar(1, pTarget, iNr) = iTime;
	EffectVar(2, pTarget, iNr) = fMessage;
	EffectVar(3, pTarget, iNr) = szCall;
	SetCrewEnabled(false, pTarget);
	return true;
}

global func FxCMCWaitTimer(object pTarget, int iNr, int iTime)
{
	if(g_TutorialObject != EffectVar(0, pTarget, iNr) || iTime == EffectVar(1, pTarget, iNr))
		return -1;
	
	if(EffectVar(2, pTarget, iNr))
		PlayerMessage(GetOwner(pTarget), "<c ff0000>%d Sekunden bis zum Start</c>", 0, (EffectVar(1, pTarget, iNr)-iTime)/35);
	
	if(!((EffectVar(1, pTarget, iNr) - iTime) % 35) && Inside((EffectVar(1, pTarget, iNr) - iTime) / 35, 1, 5))
		Sound("Ding", true, 0, 100, GetOwner(pTarget));
	
	return true;
}

global func FxCMCWaitStop(object pTarget, int iNr)
{
	SetCrewEnabled(true, pTarget);
	SetCursor(GetOwner(pTarget), pTarget, true, true);
	
	if(EffectVar(3, pTarget, iNr))
		g_TutorialObject->eval(EffectVar(3, pTarget, iNr));
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////

global func StartTimer(int iTime, string szFunc, object pCallObj, bool fScenario)
{
	return AddEffect("MinigameTmr", 0, 101, 1, 0, 0, iTime, szFunc, pCallObj, fScenario);
}

global func FxMinigameTmrStart(object target, int nr, temp, int iTime, string szFunc, object pCallObj, bool fScenario)
{
	EffectVar(0, target, nr) = iTime;
	EffectVar(1, target, nr) = szFunc;
	EffectVar(2, target, nr) = pCallObj;
	EffectVar(3, target, nr) = fScenario;
	
	if(pCallObj)
		EffectVar(4, target, nr) = true;
		
	var timerobj = FindObject(TMR_);
	if(!timerobj)
		timerobj = CreateObject(TMR_, 0, 0, -1);
		
	LocalN("start_time", timerobj) = iTime;
		
	EffectVar(5, target, nr) = timerobj;
	
	return true;
}

global func FxMinigameTmrTimer(object target, int nr)
{
	var time = --EffectVar(0, target, nr);
	var pCallObj = EffectVar(2, target, nr);
	
	if(!pCallObj && EffectVar(4, target, nr))
		return -1;
	
	EffectVar(5, target, nr)->~Time(time);
	
	if(time <= 0)
	{
		RemoveObject(EffectVar(5, target, nr));
		var szFunc = EffectVar(1, target, nr);
		if(pCallObj)
			pCallObj->Call(szFunc);
		else if(EffectVar(3, target, nr))
			GameCall(szFunc);
		else
			Call(szFunc);
			
		return -1;
	}
	
	return true;
}












