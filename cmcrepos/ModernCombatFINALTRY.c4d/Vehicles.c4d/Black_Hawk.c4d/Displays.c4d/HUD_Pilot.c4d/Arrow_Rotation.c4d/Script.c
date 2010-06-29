/* Arrow Rotation */

#strict

local hud, color;

func Initialize() 
{
  SetClrModulation(RGBa(0,200,0,50), this());
  color = "green"; 
  return(1);
}

func TimerFunc()
{
  SetVisibility(VIS_Local);

  if (LocalN("Color", hud) eq "red" && color eq "green")
  {
    SetClrModulation(RGBa(255,0,0,50), this());
    color = "red";
  }
  if (LocalN("Color", hud) eq "green" && color eq "red")
  {
    SetClrModulation(RGBa(0,200,0,50), this());
    color = "green";
  }
  if (!hud)
  {
    RemoveObject();
    return(1);
  }
  SetLocal(0, Local(0, hud), this());
  SetLocal(1, Local(1, hud), this());
  return(1);
}

