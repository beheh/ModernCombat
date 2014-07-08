/*-- Szenariosektionen --*/

#strict 2

/** Grundfunktionalitäten **/

global func SECT_SaveObject(object pObj, array &arSavedObjects)
{
	if(!pObj || GetIndexOf(pObj, arSavedObjects) > -1)
		return true;

	if(GetObjectStatus(pObj) == C4OS_NORMAL)
	{
		if((GetX(pObj) > LandscapeWidth() || GetY(pObj) > LandscapeHeight()) && !(GetCategory(pObj) & (C4D_Foreground|C4D_Parallax)) && GetProcedure(pObj) != "ATTACH")
			SetPosition(10, 10, pObj);

		if(Contained(pObj) && GetProcedure(pObj) != "ATTACH")
			pObj->Exit();

		pObj->SetObjectStatus(C4OS_INACTIVE);
		arSavedObjects[GetLength(arSavedObjects)] = pObj;
	}
	
	for(var obj in FindObjects(Find_ActionTarget(pObj)))
		if(GetProcedure(obj) == "ATTACH")
			SECT_SaveObject(obj, arSavedObjects);
	
	if(pObj->~SpecialSaveObjects())
		for(var obj in pObj->~SpecialSaveObjects())
			SECT_SaveObject(obj, arSavedObjects);
	
	return true;
}

global func SECT_LoadObjects(array arSavedObjects)
{
	for(var obj in arSavedObjects)
		if(obj)
			obj->~SetObjectStatus(C4OS_NORMAL);
		
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
		Log(Format("Loading section %s failed. arSaveObjects: (%v) -> amount: %d ; obj: (%v) ; arSavedObjects: (%v) -> length: %d", szSection, arSaveObjects, GetLength(arSaveObjects), obj, arSavedObjects, GetLength(arSavedObjects)));
		Log(Format("Used Clonk-Version: %d.%d.%d.%d [%d] (recommended: 4.9.10.4 [324])", C4X_Ver1, C4X_Ver2, C4X_Ver3, C4X_Ver4, C4X_VerBuild));
	}
	
	SECT_LoadObjects(arSavedObjects);

	return true;
}
