/*--- Sensorantenne ---*/

#strict 2
#include CSTR
#include FDSE

local launches, cooldown;

//Cooldown > 0: Nicht suchen, runterzählen bis 0
//Cooldown < 0: Suchen, runterzählen bis 30 Sekunden rum, dann Cooldown über 0 setzen
//Cooldown = 0: Suchen und bei 0 bleiben, bis Ziel gefunden

public func BuildCost() {return 25;}
public func AutoRepairDuration()	{return 0;}
public func MaxDamage()			{return 30;}
public func IsRepairable()	{return !fDestroyed;}

public func SensorDistance()	{return 190;}


public func Initialize() {return _inherited(...);}


/* Feindbewegung suchen */

protected func Sense()
{
	if(fDestroyed)
		return;

	//Das Timerintervall ist 5 Frames, Multiplikation mit 7 ergibt 35, also eine Sekunde.
	if(cooldown <= -7*30)
		cooldown = 7*10;

  
  if(cooldown != 0)
  {
  	cooldown--;
  	
  	//Bei positivem Cooldown nicht suchen
  	if(cooldown >= 0)
    	return;
  }

  //Kein Besitzer mehr: Verschwinden
  if(GetOwner() == NO_OWNER)
    return Replaced();

  //Zu markierende Gefahren suchen
  var aList =	FindObjects(Find_Distance(SensorDistance()),			//In Reichweite
  		Find_Exclude(this),						//Selber ausschließen
  		Find_NoContainer(),						//Im Freien
  		Find_Or(Find_OCF(OCF_Alive), Find_Func("IsDetectable")));	//Lebewesen oder als identifizierbar markiert;

  for (var pObj in aList)
  {
    //Nur feindliche Objekte markieren
    if(!Hostile(GetController(), GetController(pObj)))
      continue;

    //Beep
    Beep();

    //Bereits markierte Objekte auslassen
    var tag;
    if(tag = FindObject2(Find_ID(SM08), Find_Action("Attach"), Find_ActionTarget(pObj), Find_Allied(GetOwner())))
    {
      tag->~RefreshRemoveTimer(this);
      continue;
    }

    //Ansonsten markieren
    CreateObject(SM08, GetX(pObj), GetY(pObj), GetOwner())->Set(pObj, this, GetOCF(pObj) & OCF_Alive, 26);

    //Achievement-Fortschritt (Intelligence)
    DoAchievementProgress(1, AC21, GetOwner());
    
    if(!cooldown)
    	cooldown = -1;
  }


  return 1;
}

/* Beep */

public func Beep()
{
  //Erst nach Ablauf des letzten Beeps
  if(GetEffect("IntWait", this))
    return;

  //Effekte
  CreateParticle("PSpark", 1, 0, 0, 0, 80, GetPlrColorDw(GetOwner()), this);
  Sound("SNSR_Beep.ogg");

  //Kreissymbol erstellen
  CreateObject(SM09,0,0,GetOwner())->Set(this);

  //Einen Moment lang nicht mehr beepen
  AddEffect("IntWait", this, 1, 50, this);
}

public func Replaced()
{
  fDestroyed = true;

  //Verschwinden
  FadeOut(this);
}
