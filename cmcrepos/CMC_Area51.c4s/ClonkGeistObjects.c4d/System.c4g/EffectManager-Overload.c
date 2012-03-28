/*-- Effektmanager-Überladung --*/

#strict 2

global func OnUpdateEffects(int iEffect)
{
	// Kompass
  if(EFSM_Level == 3 && FindObject(CMPS))
  {
  	if(!FindObject(CMPS))
  		CHOS->SwitchCompass();
  
  	CHOS->ChangeCompassConf(0, 0, 10);
  }
  else if(EFSM_Level == 2)
  {
  	if(!FindObject(CMPS))
  		CHOS->SwitchCompass();
  	
  	CHOS->ChangeCompassConf(0, 0, 20);
  }
 	else
 	{
 		if(FindObject(CMPS))
 			CHOS->SwitchCompass();
 	}
 	
 	return _inherited(iEffect, ...);
}
