/*-- Taschenlampe --*/


#strict 2
#appendto FLSH

public func GearEffect(object pClonk)
{
  user = pClonk;
  light = AddLightCone(1000,RGBa(255,255,220,90),user);
  light->ChangeSizeXY(1900,6000);
  light->ChangeOffset(0,-2, true);
  SetAction("On");
  Sound("Click");
}

private func CheckRotation() {
  if (Contained(user)) {
    if (light)
      RemoveObject(light);
    return;
  }
  if (!light) {
    light = AddLightCone(1000,RGBa(255,255,220,90),user);
    light->ChangeSizeXY(1900,6000);
    light->ChangeOffset(0,-2, true);
  }
  light->ChangeR(user->~AimAngle(20,180));
}