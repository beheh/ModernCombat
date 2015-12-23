/*-- Gasgranate --*/

#strict 2
#include ESHL

public func BlastRadius()	{return 5;}		//Explosionsradius
protected func SecureTime()	{return 4;}		//Sicherungszeit
public func Color()		{return RGB(255,0,0);}	//Kennfarbe
func ExplodeDelay()		{return 4;}		//Verzögerung bis zu automatischer Zündung

protected func Secure()
{
  if(!active)
    return true;

  if(FrameCounter() <= start+SecureTime())
    return true;

  return false;
}

func Trigger(object pObj)
{
  //Effekte
  CastSmoke("Smoke3",12, 30, 0, 0, 100, 200, RGBa(255,255,255,100), RGBa(255,255,255,130));
  Sound("FireShellExplosion.ogg");
  Sound("GasRelease.ogg");

  //Gas erzeugen
  for(var i = 0; i < 3; i++)
  {
    var rand = Random(10) - 5;
    var flame = CreateObject(BGSE, 0,0, GetController(this));
    var xdir = GetXDir(this)/10 + rand + i*4 - i*2;
    var ydir = GetYDir(this)/10 - rand + i*4 - i*2;

    SetXDir(xdir, flame);
    SetYDir(ydir, flame);
  }

  //Verschwinden
  RemoveObject();
}

/* Timer */

func FxGrenadeTimer(object target, int effect, int time)
{
  //Bei aufgebrauchter Flugzeit explodieren
  if(time > ExplodeDelay()) return HitObject(0, true);
  var vel=Abs(GetXDir())+Abs(GetYDir());
  var alpha=Max(0,100-vel);

  //Raucheffekt
  var vel=Abs(GetXDir())+Abs(GetYDir());
  var alpha=Max(0,60-vel);
  var rgb = Color();
  if(!rgb) rgb = RGB(100,100,100);

  if(!GBackLiquid())
    CreateParticle("Smoke2", -GetXDir()/6, -GetYDir()/6, RandomX(-10, 10), -5, vel/3+RandomX(10, 20), SetRGBaValue(rgb,alpha)); 
  else
    CastObjects(FXU1,2,6);

  SetR(Angle (0,0,GetXDir(),GetYDir()));
}
