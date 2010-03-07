/*-- Gewitter --*/

#strict

local state;

protected func Initialize()
{
  SetPosition();
  
  for(var o in FindObjects(Find_ID(GetID())))
  {
    ++state;
    if(o != this())
      RemoveObject(FindObject(GetID()));
  }
  
  if(state == 1)
  {
    DoClouds(5);
	  return();
  }
  
  if(state == 2)
  {
    DoClouds(5);
    Sound("Rain.ogg",true,0,50,0,+1);
	  return();
  }
  
  if(state >= 3)
  {
    DoClouds(6);
    Sound("Rain.ogg",true,0,0,0,+1);
    return();
  }
}

protected func DoClouds(int iAmount)
{
  ClearParticles("Cloud"); 
  for(var i = LandscapeWidth()/iAmount; i > 0; i--)
  {
    var grey = Random(60);
    CreateParticle ("Cloud", i*iAmount, Random(40), 0, 0, 300+Random(600), RGBa(grey,grey,grey+1),0);
  }
}

protected func Timer()
{
  //Vom Regen-Umweltobjekt geklaut. :°
  if(state <= 1) return();
  var plr = 0;
  var range = 600;
  if(EffectLevel() >= EM4K_High)
    range = 1000;
    
  var borderleft = GetX(GetCursor(GetPlayerByIndex(plr)))-range;
  var borderright = GetX(GetCursor(GetPlayerByIndex(plr)))+range;

  for(; plr < GetPlayerCount(); plr++)
  {
    borderleft = Min(borderleft, GetX(GetCursor(GetPlayerByIndex(plr)))-range);
    borderright = Max(borderright, GetX(GetCursor(GetPlayerByIndex(plr)))+range);
  }

  CreateParticle("Raindrop", RandomX(borderleft, borderright), 0, RandomX(GetWind(0,0,1)*3, GetWind(0,0,1)*5), RandomX(200, 300), 5*64 + Random(32));
  
  if(EffectLevel() >= EM4K_High)
    CreateParticle("Raindrop", RandomX(borderleft, borderright), 0, RandomX(GetWind(0,0,1)*3, GetWind(0,0,1)*5), RandomX(200, 300), 5*64 + Random(32));

  //Blitze?
  if(state < 3) return();
  if(Random(100) - (state * 10)) return();

  var x = Random(LandscapeWidth());
  var lightning = CreateObject(FXL1,x,1,NO_OWNER);
  lightning->Activate(x, 1, -20, 41, +5, 15);
  lightning->Sound("Thunder*.ogg",1);
  lightning->AddLightFlash(1500+Random(1500),0,0,RGB(200,255,255), lightning); 
}
