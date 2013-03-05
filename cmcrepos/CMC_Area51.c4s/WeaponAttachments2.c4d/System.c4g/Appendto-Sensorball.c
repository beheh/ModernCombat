/*-- Sensorball-Appendto --*/

#strict 2
#appendto SRBL

protected func Sense()
{
  //Bei Inaktivität nicht suchen
  if(!active)
    return;

  //Kein Besitzer mehr: Verschwinden
  if(GetOwner() == NO_OWNER)
    return Remove();

  //Zu markierende Gefahren suchen
  for (var pObj in FindObjects(Find_Distance(SensorDistance()),			//In Reichweite
  		Find_Exclude(this),						//Selber ausschließen
  		Find_NoContainer(),						//Im Freien
  		Find_Or(Find_OCF(OCF_Alive), Find_Func("IsDetectable"))))	//Lebewesen oder als identifizierbar markiert
  {
    //Nur feindliche Objekte markieren
    if(!Hostile(GetController(), GetController(pObj))) continue;

    //Beep
    Beep();
    
    if(GetEffect("Silencer", Contents(0, pObj)))
      EffectVar(0, Contents(0, pObj), GetEffect("Silencer", Contents(0, pObj))) = 0;

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
