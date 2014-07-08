/*-- Sensorball --*/

#strict 2
#appendto SRBL


/* Feindbewegung suchen */

protected func Sense()
{
  //Bei Inaktivität nicht suchen
  if(!active)
    return;

  //Kein Besitzer mehr: Verschwinden
  if(GetOwner() == NO_OWNER)
    return Remove();

  //Zu markierende Gefahren suchen
  var aList =	FindObjects(Find_Distance(SensorDistance()),			//In Reichweite
  		Find_Exclude(this),						//Selber ausschließen
  		Find_NoContainer(),						//Im Freien
  		Find_Or(Find_OCF(OCF_Alive), Find_Func("IsDetectable")));	//Lebewesen oder als identifizierbar markiert;

  for (var pObj in aList)
  {
    //Nur feindliche Objekte markieren
    if(!Hostile(GetController(), GetController(pObj))) continue;

    //Beep
    Beep();

    //Bereits markierte Objekte auslassen
    var tag;
    if(tag = FindObject2(Find_ID(SM08), Find_Action("Attach"), Find_ActionTarget(pObj), Find_Allied(GetOwner())))
    {
      tag->~RefreshRemoveTimer();
      continue;
    }

    //Ansonsten markieren
    CreateObject(SM08, GetX(pObj), GetY(pObj), GetOwner())->Set(pObj, this, GetOCF(pObj) & OCF_Alive, 26);

    //Achievement-Fortschritt (Intelligence)
    DoAchievementProgress(1, AC21, GetOwner());
  }

  return 1;
}