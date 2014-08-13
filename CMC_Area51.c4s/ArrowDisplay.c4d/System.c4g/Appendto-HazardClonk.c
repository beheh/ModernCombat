/*-- Neues Script --*/

#strict 2

#appendto HZCK

/* Marker Funktionalität */

protected func Initialize()
{
	_inherited(...);
	
	AddEffect("HUDArrowCompass", this, 1, 35*10, this);
	return true;
}

public func FxHUDArrowCompassTimer(object pTarget, int iNr)
{
	var markTargets = FindObjects(Find_Func("IsMarkerTarget", this)), markCount = [];
	
	for(var target in markTargets)
	{
		var visibility = TVAR_ProcessVis(target->~TVAR_MarkerVisibility(this)), def = GetID(target), atarget;
		if(!visibility)
			visibility = TVAR_ProcessVis([TVAR_VIS_All]);
		
		if(GetIndexOf(GetOwner(), visibility) == -1)
			continue;
		
		//Objekt auf das gezeigt werden soll ermitteln
		if(target->~TVAR_MarkerTarget(this))
			atarget = target->TVAR_MarkerTarget();
		else
			atarget = target;
		
		//Distanz prüfen
		if(target->~TVAR_MarkerDistance(this))
			if(Distance(GetX(), GetY(), GetX(atarget), GetY(atarget)) < target->~TVAR_MarkerDistance(this))
				continue;
		
		//Maximalanzahl prüfen
		if(def->~TVAR_MaxMarkerCount(this))
			for(var data in markCount)
				if(def == data[0] && data[1] >= def->~TVAR_MaxMarkerCount(this))
					continue;
		
		//Pfeilobjekt erstellen
		var arrow = CreateObject(TVAR, 0, 0, GetOwner());
		arrow->Set(target->~TVAR_MarkerIcon(), this, target->~TVAR_MarkerColor());
		UpdateCompass(arrow, atarget);
	}
	/*if(FindObject(GDAR))
	{
		if(GetEffect("DataRunComputer", this))
		{
			for(var object in FindObjects(Find_Func("IsDataTransmitter")))
			{
				var arrow = CreateObject(TVAR, 0, 0, GetOwner());
				arrow->Set("Transmitter", this, RGB(255, 0, 0));
				//SetGraphics("Transmitter", this, TVAR, layer, GFXOV_MODE_Base);			
				UpdateCompass(arrow, object);
			}
		}
		else
		{
			var arrow = CreateObject(TVAR, 0, 0, GetOwner());
			arrow->Set("Computer", this, RGB(0, 0, 255));
			//SetGraphics("Computer", this, TVAR, layer, GFXOV_MODE_Base);
			var obj;
			
			if(FindObject(COMP))
				obj = FindObject(COMP);
			else
				for(var object in FindObjects(Find_OCF(OCF_CrewMember)))
					if(GetEffect("DataRunComputer", object))
						obj = object;
			
			UpdateCompass(arrow, obj);
		}
	}*/

	return true;
}

public func UpdateCompass(object pArrow, object pObj)
{
	var viewrange = 220;// GetObjPlrViewRange(this);
	var diff = Distance(GetX(), GetY(), GetX(pObj), GetY(pObj)) - viewrange;
	if(diff <= 0)
		return RemoveObject(pArrow);

	var angle = Angle(GetX(), GetY(), GetX(pObj), GetY(pObj), 1000);
  pArrow->SetPos(angle, viewrange);

  FadeOut(pArrow, 0, 5000);
  return true;
}
