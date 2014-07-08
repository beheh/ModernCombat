/*-- Vogel --*/

//Vögeln wird zufällig eine andere Grafik zugewiesen.

#strict 2

#appendto BIRD

local fHasNewSkin;


/* Initialisierung */

public func Initialize()
{
  if(!Random(20))
  {
    SetGraphics(0, this, BRSN);
    fHasNewSkin = true;
  }

  return inherited();
}

/* Tod */

protected func Death()
{
  if(GetKiller() != NO_OWNER)
  {
    var killer = GetKiller();
    var progress = GetAchievementExtra(AC53, killer);
    if(!progress)
      progress = CreateArray(2);
    if(!fHasNewSkin)
      progress[0]++;
    else
      progress[1]++;
    SetAchievementExtra(progress, AC53, killer);

    if(progress[0] > 1 && progress[1] > 0)
      //Achievement-Fortschritt (Angry Birds)
      AwardAchievement(AC53, killer);
  }

  _inherited(...);
}