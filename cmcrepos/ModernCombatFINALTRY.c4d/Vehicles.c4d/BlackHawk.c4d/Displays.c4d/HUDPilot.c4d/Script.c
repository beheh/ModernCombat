/* HUD */

#strict 2

static const BHUD_Ready = 0;
static const BHUD_Warning = 1;
static const BHUD_Error = 2;
static const BHUD_Disabled = 3;

local pHelicopter;
local iState;
local pRotation, pThrottle, pAltitude, pWind;
local fDamage, iDamageRemaining;

public func Initialize() 
{
  SetVisibility(VIS_None);
  SetState(BHUD_Ready);
  return true;
}

public func SetState(int iNewState) {
	iState = iNewState;
	var dwArrowColor;
	if(iState == BHUD_Error)
	{
		SetClrModulation(RGBa(255,0,0,50));
		dwArrowColor = RGBa(255,0,0,50);
		Sound("DamageWarning", false, pHelicopter, 100, GetOwner()+1, +1);
	}
	else {
		Sound("DamageWarning", false, pHelicopter, 100, GetOwner()+1, -1);
	}
	if(iState == BHUD_Warning)
	{
		SetClrModulation(RGBa(255,153,0,50));
		dwArrowColor = RGBa(255,153,0,50);
		Sound("WarningLockon.ogg", false, pHelicopter, 100, GetOwner()+1, +1);
	}
	else {
		Sound("WarningLockon.ogg", false, pHelicopter, 100, GetOwner()+1, -1);
	}
	if(iState == BHUD_Ready)
	{
		SetClrModulation(RGBa(0,153,255,50));
		dwArrowColor = RGBa(255,204,0,50);
	}
	if(iState == BHUD_Disabled)
	{
		SetClrModulation(RGBa(122,122,122,50));
		dwArrowColor = RGBa(255,204,0,50);
	}
	if(pRotation) pRotation->SetClrModulation(dwArrowColor);
	if(pThrottle) pThrottle->SetClrModulation(dwArrowColor);
	if(pAltitude) pAltitude->SetClrModulation(dwArrowColor);
	if(pWind) pWind->SetClrModulation(dwArrowColor);
	return true;
}

public func SetHelicopter(object pNewHelicopter) {
	pHelicopter = pNewHelicopter;
	Align();
	return true;
}

public func DamageReceived()
{
	fDamage = true;
	iDamageRemaining = 10;
	return true;
}

protected func Align() {
	SetPosition(BoundBy(GetX(pHelicopter), GetDefWidth(GetID())/2, LandscapeWidth()-GetDefWidth(GetID())/2),
		          BoundBy(GetY(pHelicopter), GetDefHeight(GetID())/2, LandscapeHeight()-GetDefHeight(GetID())/2));
	return true;
}

protected func Timer()
{
	if(!pHelicopter || !pHelicopter->GetPilot()) {
		RemoveObject();
		return true;
	}
	//Ausrichten
	Align();
	//Rotation anzeigen
	if(!pRotation)
	{
		pRotation = CreateObject(BRTN,0,0,GetOwner());
		pRotation->SetOwner(GetOwner());
		pRotation->SetVisibility(VIS_Owner);
		pRotation->SetClrModulation(RGBa(255,204,0,50));
	}
	SetPosition(GetX(), GetY()+56, pRotation);
	pRotation->SetR(GetR(pHelicopter));
	/*if(GetAction(pHelicopter) == "Turn") {
		pRotation->SetObjDrawTransform(Abs(((GetActTime(pHelicopter)-40/2)*1000)/40*2),0,0,0,1000,0);
	}
	else {
		pRotation->SetObjDrawTransform(1000,0,0,0,1000,0);
	}*/
	//Throttle anzeigen
	if(!pThrottle) {
		pThrottle = CreateObject(BARW,0,0,GetOwner());
		pThrottle->SetOwner(GetOwner());
		pThrottle->SetVisibility(VIS_Owner);
		pThrottle->SetClrModulation(RGBa(255,204,0,50));
	}
	SetPosition(GetX()-180, GetY()+70-BoundBy((((140*1000)/max_throttle)*pHelicopter->GetThrottle())/1000, 0, 140), pThrottle);
	//H�he anzeigen
	if(!pAltitude) {
		pAltitude = CreateObject(BARW,0,0,GetOwner());
		pAltitude->SetR(180);
		pAltitude->SetOwner(GetOwner());
		pAltitude->SetVisibility(VIS_Owner);
		pAltitude->SetClrModulation(RGBa(255,204,0,50));
	}
	/*var iDist = GetY(pHelicopter);
	while(!pHelicopter->GBackSolid(0, iDist) && !pHelicopter->GBackLiquid(0, iDist) && iDist < LandscapeHeight()) {
		iDist += 5;
	}
	SetPosition(GetX()+180, GetY()+64-BoundBy((((140*1000)/(GetY(pHelicopter)+iDist))*iDist)/1000, 0, 140), pAltitude);*/
	SetPosition(GetX()+180, GetY()+64-BoundBy(140-((((140*1000)/LandscapeHeight())*GetY(pHelicopter))/1000), 0, 140), pAltitude);
	//Wind anzeigen
	if(!pWind) {
		pWind = CreateObject(BARW,0,0,GetOwner());
		pWind->SetR(90);
		pWind->SetOwner(GetOwner());
		pWind->SetVisibility(VIS_Owner);
		pWind->SetClrModulation(RGBa(255,204,0,50));
	}
	SetPosition(GetX()-4+BoundBy((1400*GetWind(AbsX(GetX(pHelicopter)), AbsY(GetY(pHelicopter))))/1000, -69, 71), GetY()-98, pWind);
	//Status setzen
	if(fDamage || pHelicopter->GetDamage() >= pHelicopter->MaxDamage()*3/4) {
		SetState(BHUD_Error);	
	}
	else {
		if(pHelicopter->GetRocket()) {
			SetState(BHUD_Warning);
		}
		else {
			if(pHelicopter->GetAutopilot()) {
				SetState(BHUD_Disabled);
			}
			else {
				SetState(BHUD_Ready);
			}
		}
	}
	if(iDamageRemaining > 0) iDamageRemaining--;
	if(!iDamageRemaining) fDamage = false;
	//Sicht
  SetVisibility(VIS_Owner);
	return true;
}

public func Destruction() {
	SetState(BHUD_Ready);
	if(pRotation) RemoveObject(pRotation);
	if(pThrottle) RemoveObject(pThrottle);
	if(pAltitude) RemoveObject(pAltitude);
	if(pWind) RemoveObject(pWind);
	return true;
}
