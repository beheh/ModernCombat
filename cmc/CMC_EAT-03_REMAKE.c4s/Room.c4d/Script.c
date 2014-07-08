/*-- Raum --*/

#strict 2
#include DOOR


/* Sounds */

private func SoundOpenDoor()
{
  Sound("DOOR_Open.ogg");
}

private func SoundCloseDoor()
{
  Sound("DOOR_Close.ogg");
}

func Collection2(object pObj)
{
  g_TutorialObject->ExitArrived(pObj);
  return _inherited(...);
}
