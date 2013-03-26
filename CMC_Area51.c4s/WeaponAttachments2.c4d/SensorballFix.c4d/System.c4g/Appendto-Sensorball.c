/*-- Sensorball --*/

#strict 2
#appendto SRBL


/* Feindbewegung suchen */

protected func Sense()
{
  //Bei Inaktivit�t nicht suchen
  if(!active)
    return;

  //Kein Besitzer mehr: Verschwinden
  if(GetOwner() == NO_OWNER)
    return Remove();

  //Zu markierende Gefahren suchen
  var aList =	FindObjects(Find_Distance(SensorDistance()),			//In Reichweite
  		Find_Exclude(this),						//Selber ausschlie�en
  		Find_NoContainer(),						//Im Freien
  		Find_Or(Find_OCF(OCF_Alive), Find_Func("IsDetectable")));	//Lebewesen oder als identifizierbar markiert;

  for (var pObj in aList)
  {
    //Nur feindliche Objekte markieren
    if(!Hostile(GetController(), GetController(pObj))) continue;

    //Beep
    Beep();

    //Bereits markierte Objekte auslassen
    if(FindObject2(Find_ID(SM08), Find_Action("Attach"), Find_ActionTarget(pObj), Find_Allied(GetOwner())))
      continue;

    //Ansonsten markieren
    CreateObject(SM08, GetX(pObj), GetY(pObj), GetOwner())->Set(pObj, this, GetOCF(pObj) & OCF_Alive);

    //Achievement-Fortschritt (Intelligence)
    DoAchievementProgress(1, AC21, GetOwner());
  }

  return 1;
}