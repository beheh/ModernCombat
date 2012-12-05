/*-- Beat Box --*/

#strict 2

local transparent;


private func Swap()
{
  if(transparent)
  {
    SetSolidMask(1,0,42,40,0,1);
    transparent = false;
    SetAction("FadeIn");
    ScheduleCall(0, "Blink", 130);
    ScheduleCall(0, "Blink", 110);
    ScheduleCall(0, "Blink", 90);
  }
  else
  {
    SetSolidMask();
    transparent = true;
    SetAction("FadeOut");
  }
}

private func Blink()
{
  CreateParticle("FapLight",0,0,0,0,200,RGBa(0,250,250,0),this);
}

private func ChangeSide()
{
  SetClrModulation(RGBa(250,0,0,0));
  SetSolidMask();
  transparent = true;

  SetAction("FadeOut");
  SetPhase(4);
}