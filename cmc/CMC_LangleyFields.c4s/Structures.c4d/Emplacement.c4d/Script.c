/*-- Stellung --*/

#strict 2
#include CCBS
#include BT06

local mg_station1, mg_station2;

public func TechLevel()				{return TECHLEVEL_1;}	//Benötigte Techstufe
public func RequiredEnergy()			{return 60;}		//Energieverbraucher
public func MaxDamage()				{return 200;}		//Maximaler Schadenswert bis zur Zerstörung

public func HideWeaponRack(object pTarget)	{return true;}		//Waffen dauerhaft unsichtbar


/* Initalisierung */

protected func Initialize()
{
  //Waffen erstellen
  CreateWeapons();

  //Sucheffekt
  AddEffect("SearchTargets", this, 1, 3, this);

  //Effekte
  var deco = CreateObject(OFLG, -19, -3);
  SetOwner(GetOwner(), deco);
  AddObject(deco);

  if(HasEnergy())
    Sound("Building_PowerOn.ogg");
  else
    Sound("Building_PowerOff.ogg");

  return _inherited(...);
}

/* Feindsuche */

public func EnemySearchRange()	{return 100;}	//Suchreichweite in Schussrichtung

public func FxSearchTargetsTimer(object pTarget, int iNr)
{
  var wpr = mg_station1->GetAttWeapon(), wpl = mg_station2->GetAttWeapon();

  //Munitionsstände auffüllen
  if(wpr->GetAmmo() < wpr->~GetFMData(FM_AmmoLoad))
    wpr->DoAmmo2(0, wpr->~GetFMData(FM_AmmoID), wpr->~GetFMData(FM_AmmoLoad)-wpr->GetAmmo());
  if(wpl->GetAmmo() < wpl->~GetFMData(FM_AmmoLoad))
    wpl->DoAmmo2(0, wpl->~GetFMData(FM_AmmoID), wpl->~GetFMData(FM_AmmoLoad)-wpl->GetAmmo());

  //Ziel suchen
  var tl, tr, preTargets = FindObjects(Find_OnLine(-EnemySearchRange(), 0, EnemySearchRange(), 0), 
  			Find_NoContainer(),
  			Find_Or(
  			Find_Func("IsBulletTarget",GetID(mg_station1),0,mg_station1),
  			Find_OCF(OCF_Alive)),
  			Sort_Distance());

  for(var pT in preTargets)
  {
    //Ziel gefunden?
    if(tl || tr)
      break;

    var ox = GetX(pT);
    var oy = GetY(pT);
    
    //Befindet sich links/rechts bereits ein Ziel: Nicht zusätzlich prüfen
    if((tl && ox < GetX()) || (tr && ox > GetX()))
      continue;

    if(!CheckEnemy(pT, mg_station1, true))
      continue;

    //Pfad frei
    if(!PathFree(GetX(),GetY(),ox,oy))
      continue;

    //Unsichtbare Ziele
    if(!CheckVisibility(pT, mg_station1))
      continue;

    //Als linkes/rechtes Ziel festlegen
    if(ox > GetX())
      tr = pT;
    else if(ox < GetX())
      tl = pT;
  }

  //Linkes Geschütz starten/stoppen
  if(tl)
  {
    if(!wpl->IsRecharging())
      wpl->Fire(mg_station2);
  }
  else if(wpl->IsRecharging() && !tl)
    wpl->StopAutoFire();

  //Rechtes Geschütz starten/stoppen
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
  //Waffengestelle platzieren und ausrüsten
  mg_station1 = CreateObject(WNK2, -23, 5, GetOwner());
  mg_station1->Set(this, 0, 90, 90, 90);
  mg_station1->Arm(ACCN);
  mg_station2 = CreateObject(WNK2, 23, 5, GetOwner());
  mg_station2->Set(this, 0, -90, -90, -90);
  mg_station2->Arm(ACCN);

  //Der Objektliste hinzufügen
  AddObject(mg_station1);
  AddObject(mg_station2);

  return true;
}