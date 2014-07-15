/*-- Stellung --*/

#strict 2
#include CCBS
#include BT01

local mg_station1, mg_station2;

public func TechLevel()		{return TECHLEVEL_1;}	//Ben�tigte Techstufe
public func RequiredEnergy()	{return 60;}		//Energieverbraucher
public func MaxDamage()		{return 200;}		//Maximaler Schadenswert bis zur Zerst�rung

public func HideWeaponRack(object pTarget) {return true;} //Waffen dauerhaft unsichtbar

/* Initalisierung */

protected func Initialize()
{
  //Waffen erstellen
  CreateWeapons();

  //Sucheffekt
  AddEffect("SearchTargets", this, 1, 3, this);

  //Effekte
  if(HasEnergy())
    Sound("Building_PowerOn.ogg");
  else
    Sound("Building_PowerOff.ogg");

  return _inherited(...);
}

/* Sucheffekt */

public func EnemySearchRange() {return 100;} //Suchreichweite je Richtung

public func FxSearchTargetsTimer(object pTarget, int iNr)
{
  var wpr = mg_station1->GetAttWeapon(), wpl = mg_station2->GetAttWeapon();
  
  //Munitionsst�nde auff�llen
  if(wpr->GetAmmo() < wpr->~GetFMData(FM_AmmoLoad))
    wpr->DoAmmo2(0, wpr->~GetFMData(FM_AmmoID), wpr->~GetFMData(FM_AmmoLoad)-wpr->GetAmmo());
  if(wpl->GetAmmo() < wpl->~GetFMData(FM_AmmoLoad))
    wpl->DoAmmo2(0, wpl->~GetFMData(FM_AmmoID), wpl->~GetFMData(FM_AmmoLoad)-wpl->GetAmmo());

  var tl, tr, preTargets = FindObjects(Find_OnLine(-EnemySearchRange(), 0, EnemySearchRange(), 0), 
                            Find_NoContainer(),
                            Find_Or(
                              Find_Func("IsBulletTarget",GetID(mg_station1),0,mg_station1),
                              Find_OCF(OCF_Alive)),
		                        Sort_Distance());
	
	for(var pT in preTargets)
	{
	  //Schon alle Ziele gefunden?
	  if(tl && tr)
	    break;
	
		var ox = GetX(pT);
		var oy = GetY(pT);
		
		//Befindet sich links/rechts wo schon ein g�ltiges Ziel ist? Dann nicht zus�tzlich pr�fen
		if((tl && ox < GetX()) || (tr && ox > GetX()))
		  continue;
	
		if(!CheckEnemy(pT, mg_station1, true))
      continue;
	
		// Pfad frei
		if(!PathFree(GetX(),GetY(),ox,oy))
			continue;
		
		// unsichtbare Ziele
		if(!CheckVisibility(pT, mg_station1))
			continue;
		
		//Als linkes/rechtes Ziel festlegen
		if(ox > GetX())
		  tr = pT;
		else if(ox < GetX())
      tl = pT;
	}
	
	//Linkes Gesch�tz starten/stoppen
	if(tl)
	{
	  if(!wpl->IsRecharging())
	    wpl->Fire(mg_station2);
	}
	else if(wpl->IsRecharging() && !tl)
	  wpl->StopAutoFire();
	
	//Rechtes Gesch�tz starten/stoppen
	if(tr)
	{
	  if(!wpr->IsRecharging())
	    wpr->Fire(mg_station1);
	}
	else if(wpr->IsRecharging() && !tr)
	  wpr->StopAutoFire();
	
	return true;
}

/* Waffenerstellung */

public func CreateWeapons()
{
  mg_station1 = CreateObject(WNK2, 0, 0, GetOwner());
  mg_station1->Set(this, 0, 90, 90, 90);
  mg_station1->Arm(ACCN);
  mg_station2 = CreateObject(WNK2, 0, 0, GetOwner());
  mg_station2->Set(this, 0, -90, -90, -90);
  mg_station2->Arm(ACCN);
  
  return true;
}
