/*-- Sensoreinheit --*/

#strict 2

#include CCUS

public func ResearchCost()			{return 140;}	//Entwicklungskosten
public func ResearchDuration()			{return 35*8;}	//Entwicklungsdauer in Frames

public func ResearchBase()			{return [];}	//Upgrades, die bereits erforscht sein müssen

public func OnUpgrade(object pBy)
{
  AddEffect("SensorUnit", pBy, 100, 5, 0, U_SU);
  
  return true;
}

public func FxSensorUnitStart(object pTarget, int iNr, int iTemp)
{
  if(iTemp)
    return;

  EffectVar(0, pTarget, iNr) = 0;
  return true;
}

public func FxSensorUnitTimer(object pTarget, int iNr)
{
  Sense(pTarget);
  
  return true;
}

/* Sensor */

public func SensorDistance() { return 200; }

protected func Sense(object building)
{
  //Zu markierende Gefahren suchen
  for(var pObj in building->FindObjects(building->Find_Distance(SensorDistance()),					//In Reichweite
  				Find_Exclude(building),						//Selber ausschließen
  				Find_NoContainer(),						//Im Freien
  				Find_Hostile(GetOwner(building)),					//Nur Feinde
  				Find_Or(Find_OCF(OCF_Alive), Find_Func("IsDetectable"))))	//Lebewesen oder als identifizierbar markiert
  {
    //Beep.
    Beep(building);

    //Nicht markieren wenn schon der Fall
    var tag;
    if(tag = building->FindObject2(Find_ID(SM08), Find_Action("Attach"), Find_ActionTarget(pObj), Find_Allied(GetOwner(building))))
    {
      tag->~RefreshRemoveTimer();
        continue;
    }

    //Ansonsten markieren
    CreateObject(SM08, GetX(pObj), GetY(pObj), GetOwner())->Set(pObj, building, GetOCF(pObj) & OCF_Alive, 26);

    //Achievement-Fortschritt (Intelligence)
    DoAchievementProgress(1, AC21, GetOwner(building));
  }
  return 1;
}

/* Beep */

public func Beep(object building)
{
  //Erst nach Ablauf des letzten Beeps
  if(GetEffect("IntWait", building))
    return;

  //Effekte
  CreateParticle("PSpark", 0, 0, 0, 0, 60, GetPlrColorDw(GetOwner(building)), building);

  //Kreissymbol erstellen
  CreateObject(SM09, 0, 0, GetOwner(building))->Set(building);

  //Einen Moment lang nicht mehr beepen
  AddEffect("IntWait", building, 1, 50, building);
}
