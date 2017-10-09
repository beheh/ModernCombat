/*-- Bodenplatte --*/

#strict 2
#include PANE

public func MaxDamage()	{return 50;}


/* Zerstörung */

public func Shatter(int iPower, object pFrom)
{
  if(damaged) return;
  damaged = true;

  if(!iPower) iPower = 20;
  var angle = 0;
  var spread = 180;

  if(pFrom)
  {
    angle = ObjectAngle(pFrom);
    spread = 60;
  }

  //Zersplittern
  CastParticles("Glas",3+Random(3),50,0,0,60,10);
  for(var i = 0 ; i < GetDefWidth()/3 ; i++)
    CreateParticle("GlassSplinter",i*3-GetDefWidth()/2,0,+Sin(angle+(Random(spread)-spread/2),iPower),-Cos(angle+(Random(spread)-spread/2),iPower),20);
  Sound("GlassBreak*.ogg");

  RemoveObject();
}