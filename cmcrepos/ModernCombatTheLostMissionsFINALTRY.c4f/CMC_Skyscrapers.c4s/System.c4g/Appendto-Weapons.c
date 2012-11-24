/*-- Waffen --*/

//Bestimmte Waffen werden weggeworfen, sobald sie leergefeuert wurden und k�nnen nicht mehr nach- oder entladen werden.

#strict 2
#appendto RVLR
#appendto PPGN
#appendto MNGN

local empty;


public func OnEmpty()
{
  //Waffe leer
  empty = true;

  ThrowAway();
}

/* Wegwurf */

public func ThrowAway()
{
  //Kategorie wechseln
  SetCategory(C4D_Vehicle);
  //Waffe auswerfen sofern verschachtelt
  if(Contained())
  {
    //Sch�tzen verlassen
    var dir = +1;
    if(GetDir(GetUser()) == DIR_Right)
      dir = -1;

    //Tr�ger stellt zielen ein sofern n�tig
    if(Contained()->~IsAiming() && Contents(0, Contained()) == this)
      Contained()->~StopAiming();

    Exit(0, 0, 0, Random((360)+1), dir,-3, Random(11)-5);

    Sound("AT4R_ThrowAway.ogg");
  }

  //Verschwinden
  FadeOut();
  
  return true;
}

/* Aufnahme */

public func RejectEntrance()
{
  if(empty)
    return true;
}

/* Lademechanismen deaktivieren */

public func FMData1(int data)
{
  if(data == FM_NoAmmoModify)	return 1;

  return _inherited(data, ...);
}