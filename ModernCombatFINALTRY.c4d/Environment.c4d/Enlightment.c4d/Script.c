/*-- Lichteinfall --*/

#strict 2

local fEnlightment;


/* Initialisierung */

func Initialize() 
{
  SetPosition(0,0);
  fEnlightment = false;
}

/* Lichterstellung */

public func Enlight()
{
  //Nicht wenn Dunkel oder Nacht
  if(IsNight() || GetDarkness() > 0) 
  {
    fEnlightment = false;
    return;
  }

  fEnlightment = true; 

  //Einheitliche Rotation
  var CurrentR = RandomX(160, 200);
  //var Red, Green, Blue;
  //X-Größe
  var SizeX = RandomX(50,100);
  //Platzieren nach Landschaftsbreite
  for(var i = 0; i < LandscapeWidth()/250; ++i) 
  {
    var holder = CreateObject(TIM1, Random(LandscapeWidth()), -10, -1);
    var lite = AddLightCone(1000, RGB(255,255,255), holder);

    //Berechnen, ab wo kein Material mehr ist
    var SizeY = 0;
    var iY = 0;
    while(!(lite->GBackSolid(GetX(), GetY()+iY)))
    {
      SizeY += 5;
      ++iY;
    }

    lite->ChangeSizeXY(SizeX, SizeY);
    lite->ChangeR(CurrentR);
    lite->TurnOn();
    lite->FadeIn();

    //Effekt zur Erkennung anhängen
    AddEffect("EnvLight", lite, 50, 100, this, ELGT);
    AddEffect("EnvLight", holder, 50, 100, this, ELGT);

    if(SizeY < 100)
      RemoveObject(holder);
   }
  return 1;
}

func FxEnvLightTimer(object pTarget)	{}

private func Check()
{
  if(IsNight() || GetDarkness() > 0 || !GetEffectData(EFSM_Enlight)) {
    Log("remove");
    if(fEnlightment == true)
      for(var pObj in FindObjects(Find_ID(TIM1), Find_ID(LGHC))) 
      {
        if(GetEffect("EnvLight", pObj)) 
        {
          pObj->FadeOut();
          fEnlightment = false;
        }
      }
     }
  if(fEnlightment == false) 
    if(!IsNight()) 
      if(GetDarkness() == 0) 
        return Enlight();
}