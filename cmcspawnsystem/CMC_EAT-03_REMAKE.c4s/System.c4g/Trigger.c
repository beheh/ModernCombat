/*-- Triggersystem / Auslösersystem --*/

#strict 2

/* Mit dem Triggersystem soll es möglich sei, Objekten bestimmte Trigger zuzuweisen.
 * Diese würden dann in einem Objekt, im Szenario oder Global eine Funktion
 * aufrufen.
 */
 
/** TriggerTypes **/

static const TRIGGER_Find = 1; // Bei gefundenen Objekten. ExtraOptions: [[Find_Mask, Find_Mask, ...], fRefind] (Bsp: [[Find_ID(PCMK), Find_Action("Baum")]]) | Parameter: (array pObjects)
static const TRIGGER_Collection = 2; // Falls das Objekt etwas aufhebt: [[ID, ID, ID, ...] bzw [Object, Object, Object, ...] -> Welche lösen das aus.] | Paramter: (object pObj)
static const TRIGGER_Entrance = 3; // Falls das Objekt ein anderes Objekt betritt: [[ID, ID, ID, ...]  bzw [Object, Object, Object, ...] -> Welche lösen das aus.] | Paramter: (object pContainer)
static const TRIGGER_Removed = 4; // Falls das Objekt entfernt wird: [] | Paramter: ()
static const TRIGGER_Func = 5; // Falls eine bestimmte Scriptausführung iVal zurückgibt: [szScript, iVal] | Paramter: ()
static const TRIGGER_Departure = 6; // Falls das Objekt ein anderes Objekt verlassen hat: [[ID, ID, ID, ...]  bzw [Object, Object, Object, ...] -> Welche lösen das aus.] | Paramter: (object pContainer)
static const TRIGGER_Control = 7; // Falls das Objekt von einem anderem Objekt kontrolliert wird: [[ID, ID, ID, ...] bzw [Object, Object, Object, ...], [szControl, szControl, ....]] ACHTUNG: Es gilt sowohl Control_ als auch Contained_! | Paramter: (object pCaller, string szCtrl)
static const TRIGGER_Damage = 8; // Falls das Objekt von einem anderem Objekt beschädigt wird: [iMinDmgChange, [iDmgType, iDmgType, ...], [ID/Object, ...]] | Paramter: (iDamage, iDmgType, object pByObj)
static const TRIGGER_Activate = 9; // Falls das Objekt von einem anderem Objekt aktiviert wird: [[ID, ID, ID, ...] bzw [Object, Object, Object, ...]] | Paramter: (object pByObj)
static const TRIGGER_Grabbed = 10; // Falls das Objekt von einem anderem Objekt angefasst wird: [[ID, ID, ID, ...] bzw [Object, Object, Object, ...]] | Paramter: (object pByObj, bool fGrab)
static const TRIGGER_Death = 11; // Falls das Objekt stirbt: [] | Paramter: ()

/** Functions **/

/**
 * pTarget: Das Objekt, an dem der Trigger gebunden werden soll. (Bei lokalem Aufruf das Objekt selbst.)
 * iTriggerType: Der Triggertyp.
 * arExtraOptions: In einem Array angegebene Extrafunktionen je nach Triggertyp. (s.o.)
 * szFunc: Die Funktion, die aufgerufen werden soll. Falls keine Parameter angegeben werden, so werden die jeweiligen Triggertyp-Parameter weitergegeben. (s.o.)
 * pCallObj: Ruft die Funktion in einem Objekt auf. Falls 0, wird die Funktion global oder im Szenario aufgerufen.
 * fScenario: Falls pTarget 0 ist und dieser Wert true ist, wird die Funktion im Szenario aufgerufen. Ansonsten global.
 **/
 
// Aktualisierungsrate des Trigger-Effekts
static const TRIGGER_UpdateRate = 10;

 
global func BindTrigger(object pTarget, int iTriggerType, array arExtraOptions, string szFunc, object pCallObj, bool fScenario, bool fRemove)
{
	if(!pTarget && !(pTarget = this))
		return false;
	
	return AddEffect("CMCBindTrigger", pTarget, 101, TRIGGER_UpdateRate, 0, 0, [iTriggerType, arExtraOptions, szFunc, pCallObj, fScenario, fRemove]);
}

global func RemoveTrigger(object pTarget, int iTriggerType)
{
	var i, effect;
	while(effect = GetEffect("CMCBindTrigger", pTarget, i))
	{
		if(iTriggerType == EffectVar(0, pTarget, effect))
			return RemoveEffect("CMCBindTrigger", pTarget, i);
	
		++i;
	}
}

/** OMGWTF - Triggereffekt **/

global func FxCMCBindTriggerStart(object pTarget, int iNr, temp, array arOptions)
{
	if(temp)
		return -1;
		
	EffectVar(0, pTarget, iNr) = arOptions[0]; // Triggertyp
	EffectVar(1, pTarget, iNr) = arOptions[1]; // Extra Optionen
	EffectVar(2, pTarget, iNr) = arOptions[2]; // Aufgerufene Funktion
	EffectVar(3, pTarget, iNr) = arOptions[3]; // Call-Object
	EffectVar(4, pTarget, iNr) = arOptions[4]; // fScenario
	
	EffectVar(5, pTarget, iNr) = []; // Zusatzarray für TRIGGER_Find
	
	for(var i = 0; i < GetLength(arOptions[2]); i++)
	{
		if(GetChar(arOptions[2], i) == 40)	
		{
			EffectVar(6, pTarget, iNr) = true;
			break;
		}
	}
	
	EffectVar(7, pTarget, iNr) = arOptions[5];
	
	return true;
}

global func FxCMCBindTriggerTimer(object pTarget, int iNr, int iTime, bool fRemove)
{
	var type = EffectVar(0, pTarget, iNr);
	var options = EffectVar(1, pTarget, iNr);
		
	if(type == TRIGGER_Find)
	{
		var objects = pTarget->FindObjects(options[0][0], options[0][1], options[0][2], options[0][3], options[0][4], options[0][5], options[0][6], options[0][7], options[0][8], options[0][9]);
		if(GetLength(objects))
		{
			if(options[1])
				EffectCall(pTarget, iNr, "CallFunction", objects);
			else
			{
				// Ansonsten alle schon gewesenen Objekte rausfiltern.
				var func_objects = [];
				for(var obj in objects)
					if(GetIndexOf(obj, EffectVar(5, pTarget, iNr)) == -1)
						func_objects[GetLength(func_objects)] = obj;
				
				if(!GetLength(func_objects))
					return false;
				
				EffectVar(5, pTarget, iNr) = objects;
				EffectCall(pTarget, iNr, "CallFunction", func_objects);
			}
		}
	}
	else if(type == TRIGGER_Func)
	{
		if(eval(options[0]) == options[1])
			EffectCall(pTarget, iNr, "CallFunction");
	}
	
	return true;
}

global func FxCMCBindTriggerCallFunction(object pTarget, int iNr, par1, par2, par3, par4, par5, par6, par7)
{
	var szFunc = EffectVar(2, pTarget, iNr);
	var pCallObject = EffectVar(3, pTarget, iNr);
	var fScenario = EffectVar(4, pTarget, iNr);
	
	var iType = EffectVar(0, pTarget, iNr);
	var options = EffectVar(1, pTarget, iNr);
	
	if(EffectVar(8, pTarget, iNr) == FrameCounter()) // "Schutz" bei überladenen Objekten, damit diese nicht X-Male diese Funktion aufrufen :/
		return false;
	
	if(iType == TRIGGER_Collection || iType == TRIGGER_Entrance || iType == TRIGGER_Departure || iType == TRIGGER_Activate || iType == TRIGGER_Grabbed)
	{
		if((GetType(options[0]) == C4V_Array && GetIndexOf(par1, options[0]) == -1) || (GetType(options[0]) == C4V_Array && GetIndexOf(GetID(par1), options[0]) == -1))
			return false;
	}
	else if(iType == TRIGGER_Control)
	{
		// par1 = pCaller, par2 = szCtrl
		if((GetType(options[0]) == C4V_Array && GetIndexOf(par1, options[0]) == -1) || (GetType(options[0]) == C4V_Array && GetIndexOf(GetID(par1), options[0]) == -1) || (GetType(options[1]) == C4V_Array && GetIndexOf(par2, options[1]) == -1))
			return false;
	}
	else if(iType == TRIGGER_Damage)
	{
		// par1 = iChange, par2 = iType, par3 = pFrom
		if(par1 < options[0] || (GetType(options[1]) == C4V_Array && GetIndexOf(par2, options[1]) == -1) || (GetType(options[2]) == C4V_Array && GetIndexOf(par3, options[2]) == -1) || (GetType(options[2]) == C4V_Array && GetIndexOf(GetID(par3), options[2]) == -1))
			return false;
	}
	
	if(EffectVar(6, pTarget, iNr))
		eval(szFunc);
	else
	{
		if(pCallObject)
			pCallObject->Call(szFunc, par1, par2, par3, par4, par5, par6, par7);
		else if(!fScenario)
			Call(szFunc, par1, par2, par3, par4, par5, par6, par7);
		else
			GameCall(szFunc, par1, par2, par3, par4, par5, par6, par7);
	}
	
	EffectVar(8, pTarget, iNr) = FrameCounter();
	
	if(EffectVar(7, pTarget, iNr))
		return RemoveEffect(0, pTarget, iNr);
	
	return true;
}
