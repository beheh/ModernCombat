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
  OpenSectionMenu(GetOwner(pObj));
  return _inherited(...);
}

func Check() {
	if(!Contents()) return;
	if(!GetMenu(Contents()) && !GetCommand(Contents())) SetCommand(Contents(), "Exit");
}
