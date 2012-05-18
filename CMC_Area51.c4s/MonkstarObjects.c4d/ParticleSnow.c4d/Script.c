/*-- Partikelschnee --*/

#strict 2

public func Initialize() 
{
  AddEffect("CreateSnow", this, 1, 10, this);
  return true;
}

public func FxCreateSnowTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var i, iX, iY;

  iX = GetX();
  iY = GetY();

  for(i = 0; i < 50; i++)
  {
    var iRand = Random(LandscapeWidth()) - iX;
    if(GBackSky(iRand, 5))
      CreateParticle("Snowflake", iRand, -iY, 0, 0, 35, 0xFFFFFF);
  }
  
  return true;
}
