/*-- Benzinfass --*/

#strict 2
#include XBRL


/* Zerstörung */

func BlowUp(int iPlr)
{
  //Effekt
  CastParticles("MetalSplinter",4,100,0,0,20,70,RGB(250,0,0));
  //Explosion
  CreateObject(ROCK,0,0,iPlr)->Explode(20);

  SetAction("Wreck");
  SetController(iPlr);
  Extinguish();
  SetRDir(RandomX(-40,+40));
  AddFireEffect(this,50,RGB(80,80,80),true,30);

  //Langsam verschwinden
  FadeOut();

  //Von NapalmGranate aus Hazard. :)
  //if(Stuck()) return();
  
  var iDamage = 25;
    
  var flamecount, strength;
  for(strength = 30; strength > 10; strength -= 9)
  {
    flamecount = 7 + Random(5);
    var ichwillnichtabstuerzen = 0;
    while(flamecount && ichwillnichtabstuerzen < 3)
    {
      ichwillnichtabstuerzen++;
      for(var z=flamecount, a = 360/z; z > 0; z--)
      {
        var flame, x,y;
        x = +Cos(a*z,strength);
        y = -Sin(a*z,strength);
        var flame = CreateObject(FFLM,x/10,y/10,iPlr);
        flame->Launch(x,y,0,iDamage/2);

        if(!Stuck(flame))
          flamecount--;
      }
    }
  }
}
