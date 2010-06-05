/*-- Kriegsflair --*/

#strict 2

local skyfade;


/* Initialisierung */

protected func Initialize()
{
  return 1;
}

/* Explosionen */

protected func Make()
{
  if(skyfade) skyfade-=20;

  if(skyfade <= 255) SetSkyAdjust(RGBa(255,255,255,220), RGB(skyfade,skyfade,skyfade));
  if(!Random(40-ObjectCount(GetID())*5) && GetEffectData(EFSM_Waratmosphere))
  {
   var x = Random(LandscapeWidth()), y = Random(LandscapeHeight()),dis=Random(100)+200;
   if(GBackSky(x,y)
    && PathFree(x,0,x,y) 
    && GBackSky(x,y+70) 
    && PathFree(x,y,x,y+70))
    {
     CreateParticle("Blast",x,y,0,0,dis*2,RGBa(100,100,100,70));
     Sound("C4EX_Detonation*.ogg",1,0,20);
    }      
  }
  if(Random(65-ObjectCount(GetID())*5)) return 1;
  skyfade = 300;
  Sound("Warflair*.ogg",1);
  return 1;
}
