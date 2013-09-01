/*-- Lichteinfall --*/

#strict 2

local fEnlightment;

static const ELGT_MaxDarkness = 350;


/* Initialisierung */

func Initialize() 
{
  //Positionieren
  SetPosition();

  fEnlightment = false;
}

/* Lichterstellung */

public func Enlight()
{
  //Nicht wenn Dunkel oder Nacht
  if(IsNight() || GetDarkness(1000) > ELGT_MaxDarkness || !GetEffectData(EFSM_Enlight))
  {
    fEnlightment = false;
    return;
  }

  fEnlightment = true; 

  //Einheitliche Rotation
  var CurrentR = 180+RandomX(10, 20)*(Random(2)*2-1);
  //X-Größe
  var SizeX = RandomX(50,100);
  //Platzierung nach Landschaftsbreite
  for(var i = 0; i < LandscapeWidth()/250; ++i) 
  {
    var holder = CreateObject(TIM1, Random(LandscapeWidth()), -10, -1);
    var lite = AddLightCone(1000, RGB(255,255,255), holder);

    //Berechnen, ab wo kein Material mehr ist
    var SizeY = 0;
    var x = GetX(holder), y = GetY(holder), xdir = 0, ydir = 0;
    SimFlight(x, y, xdir, ydir);
    SizeY = y * 6;

    lite->ChangeSizeXY(SizeX, SizeY);
    lite->ChangeR(CurrentR);
    lite->TurnOn();
    lite->SetAlpha(0);

    //Effekt zur Erkennung anhängen
    AddEffect("EnvLight", lite, 50, 100, this, ELGT);
    AddEffect("EnvLight", holder, 50, 100, this, ELGT);

    if(SizeY < 100)
      RemoveObject(holder);
   }
  return 1;
}

func FxEnvLightTimer(object pTarget)	{}

/* Timer */

private func Timer()
{
  if(FindObject(CHOS))
  {
    var activated = GetDarkness(1000) <= ELGT_MaxDarkness;
    fEnlightment = false;
    for(var pObj in FindObjects(Find_ID(LGHC)))
    {
      if(GetEffect("EnvLight", pObj))
      {
        if(activated)
          SetVisibility(VIS_All, pObj);
        else
          SetVisibility(VIS_None, pObj);

        fEnlightment = true;
      }
    }
  }
  else if(IsNight() || GetDarkness(1000) > ELGT_MaxDarkness || !GetEffectData(EFSM_Enlight))
  {
    if(fEnlightment == true)
    {
      for(var pObj in FindObjects(Find_Or(Find_ID(TIM1), Find_ID(LGHC))))
      {
        if(GetEffect("EnvLight", pObj))
        {
          pObj->FadeOut();
          fEnlightment = false;
        }
      }
    }
  }
  if(fEnlightment == false)
    if(!IsNight())
      if(GetDarkness(1000) <= ELGT_MaxDarkness || !GetEffectData(EFSM_Enlight))
        return Enlight();
}