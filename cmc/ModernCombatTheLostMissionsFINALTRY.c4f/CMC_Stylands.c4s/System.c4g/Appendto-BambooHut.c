/*-- Bambush�tte --*/

//Erweiterte Killverfolgung beim Verschieben von Bambush�tten.

#strict 2
#appendto HUT1


/* Zerst�rung */

protected func Destruction()
{
  if(GetController() != NO_OWNER)
  {
    for (var i=0; i<ContentsCount(); ++i)
      if(GetAlive(Contents(i)))
        SetKiller(GetController(), Contents(i));
      else
        SetController(GetController(), Contents(i));
  }
  _inherited(...);
}