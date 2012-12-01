/*-- Markierung  --*/

#strict 2

local ialpha;

public func Initialize() 
{ 
  SetAction("Down");
  AddEffect("FadeOut", this, 101, 1, this);
  return 1;
}

public func FxFadeOutStart(target, nr)
{
	EffectVar(0, target, nr) = [255, 127, 127];
}

public func FxFadeOutTimer(target, nr)
{
	EffectVar(1, target, nr)++;
	var clr = EffectVar(0, target, nr);
	var alpha = EffectVar(1, target, nr);
	SetClrModulation(RGBa(clr[0], clr[1], clr[2], BoundBy(alpha, 0, 255)));
	if(alpha > 255)
	{
		RemoveObject(target);
		return -1;
	}
}
