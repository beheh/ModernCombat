/*-- Waffenscript-Erweiterung für Waffenupgrades --*/

#strict 2
#appendto WPN2

local iAttachment;

static const AT_NoAttachment = 0; //Kein Attachment
static const AT_ExtendedMag  = 1; //Erweitertes Magazin
static const AT_Bayonet      = 2; //Bajonett

func SetAttachment(int iValue)
{
  return iAttachment = iValue;
}
