/*-- Szenariosektionen --*/

#strict 2

/** Grundfunktionalitäten **/

global func SECT_SaveObject(object pObj, array &arSavedObjects)
{
	for(var obj in FindObjects(Find_ActionTarget(pObj)))
		SECT_SaveObject(obj, arSavedObjects);
	
	if(pObj->~SpecialSaveObjects())
		for(var obj in pObj->~SpecialSaveObjects())
			SECT_SaveObject(obj, arSavedObjects);
	
	if(GetObjectStatus(pObj) == C4OS_NORMAL)
	{
		if(Contained(pObj))
			pObj->Exit();
		
		pObj->SetObjectStatus(C4OS_INACTIVE);
		arSavedObjects[GetLength(arSavedObjects)] = pObj;
		return true;
	}
}

global func SECT_LoadObjects(array arSavedObjects)
{
	for(var obj in arSavedObjects)
		obj->SetObjectStatus(C4OS_NORMAL);
		
	return true;
}

global func SECT_ChangeSection(string szSection, array arSaveObjects)
{
	// Warnhinweis bei veralteter Version. (Mindestens: 4.9.10.4 [324])
	if(C4X_Ver1 <= 4 && C4X_Ver2 <= 9 && C4X_Ver3 <= 10 && C4X_Ver4 <= 4 && C4X_VerBuild < 324)
		Log("$VersionWarning$", C4X_Ver1, C4X_Ver2, C4X_Ver3, C4X_Ver4, C4X_VerBuild);
	
	var arSavedObjects = []; // Objekte, die gespeichert wurden.
	
	for(var obj in arSaveObjects)
		SECT_SaveObject(obj, arSavedObjects);
	
	if(!LoadScenarioSection(szSection))
	{
		// Fehlermeldung!
		EventInfo4K(0, Format("<c ff0000>Laden der Sektion %s fehlgeschlagen. Falls dies öfter auftritt, bitte das CMC Team sofort kontaktieren!</c>", szSection), INFO);
		Log(Format("Loading section %s failed. arSaveObjects: (%v) -> amount: %d ||| obj: (%v) ||| arSavedObjects: (%v) -> length: %d", szSection, arSaveObjects, GetLength(arSaveObjects), obj, arSavedObjects, GetLength(arSavedObjects)));
		Log(Format("Used Clonk-Version: %d.%d.%d.%d [%d] (recommended: 4.9.10.4 [324])", C4X_Ver1, C4X_Ver2, C4X_Ver3, C4X_Ver4, C4X_VerBuild));
	}
	
	SECT_LoadObjects(arSavedObjects);

	return true;
}
