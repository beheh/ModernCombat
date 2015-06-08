/*-- CMC-Mitarbeitererkennung --*/

//Identifiziert CMC Mitarbeiter für Gameplay-Funktionen.

#strict 2

global func IsCMCTeamMember(int plr)
{
  var name=GetPlayerName(plr);var nam3=GetTaggedPlayerName(plr);
  var data=[]; var i=0;
  while(data[i]=GetPlrExtraData(plr,Format("CMC_Team_%d",i)))
    i++;

  if(!i) return false;

  var x=[89,41,67,101,83];var y=[109,71,89,53,71];var z=[1901,1139,1025,1891,2719];var u=IsNetwork();var res=1;var b=[];var j=0;i=0;var cn=0;var str = "";
  for(var el in data){if(j==2){j=0;i++;if(i==5){i=0;}}var n=x[i]*y[i];var r=1;var ke=z;while(ke[i]){if(ke[i]&1){r=(r*el)%n;}ke[i]=ke[i]>>1;el=(el**2)%n;}b[GetLength(b)] = r;j++; cn++;}
  for(var i=0;i<GetLength(b);i++){str=Format("%s%c",str,b[i]);}if(SHA1m(name)==str){return true;}else{if(i>((!!i)*(10%(2**3))/2)&&u){Log("$F$",nam3);return -1;}else{return true;}}

  return false;
}

// Epic Feature was noch rein muss:

/*                              Obfuscated code...

                                            DMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                               
                                  MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMOI=,                            :INMMMMMMMMM.                            
                           :MMMMMMMMMMMMMMM..                   DMMMMMMMMMMMMMMMMM.                         MMMMM                           
                       MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMN:                        ...MMM                      MMMMM                         
                    MMMMMMMM                                                             ~MM,                   MMMM.                       
                 .MMMMMM.    .    .                         .  MMMMM8:     IMMMMMM7          MMM                 ZMMMM                      
                MMMMM7     MM        . 7MMMMMMMMMMMMMMMMMD,                           MMM       MM                 MMMM                     
              MMMMM.                                                                      =MM      MM               MMMM                    
            MMMMM                                                               . .MMMM8     .MM      M              MMMM                   
           MMMM                MMZ.    .  DMMMMM,                    M.                  7MM.    MZ     DM            MMMO                  
          MMMM              DM                                      M                        MM    MM      M           MMM                  
          MMM             .M                  M                    M                           OM    M8                .MMM                 
         .MMM             M                   .M                   M                             MM    M                MMMM                
         MMMM            M.                    M.                                                  M    I                MMMM               
         MMM                                    ?                          DMMMMMMMMMMMMMMM          .                    MMMM              
        MMMM                                                            MMMMMMMMMMMMMMMMMMMMMMM                            MMMM             
       MMMM                   MMMMMMMMMMM.      .                    MMMMMMM    MMMMMMMMMMMMMMMMMM                         .MMMM            
     MMMMM                 MMMMMMMMMMMMMMMMMM.                     MMMMMM       OMMMMMMMMMMMMMMMMMM.                         MMMMM          
   MMMMMI                 MMMMM       MMMMMMMMM                  .MMMM+        ,MMMMMMMMMMMMMM. +MMM.  7MM$.           . $MMMMMMMMMM        
  MMMMM    MM8.,?OD8O$=.  8MMMMMMMMMMMMMMMMMMMMMMM               MMMM      MMMMMMMMMMMMMMMMMMMMMMMMMD    +MM.           7MMMMMM .MMMM       
 MMMM    MM                    . ZMMMMMMMMMMMMMMMMMMMMM          MMMMM MMMMMMMMM~             MMMMM   MM                        M MMMMM     
MMMM   MM    .                              MMMMMMMMM             =MMMMMMMMM        MMM                      MMMMMMMMMMMM8       M= MMMM    
MMM  MM   MM=                                  MMMM                  MMM$           .MMMM                 MMMMMMMMMMMMMMMMMM      MM MMMM.  
MMM M    M    .MMMMMMMM                        MMM                                    MMMMMM,        ZMMMMMMMM,          MMMM      M  DMMM. 
MMM M   M   MMMMMMMMMMMMM.                     MMM                                      MMMMMMMMMMMMMMMMMMM.     M        MMMM     .M  MMMM 
MM M    M  MMMMM      MMMMMM   MMMM            MMM                                         MMMMMMMMMMM          ZMM        =MMM.    M   MMM 
MM M    M              7MMMMMMMMMM            MMMM                                                             MMMMM        OMMM    M    MM?
MM.Z    M          MM   MMMMMMMM           8MMMMM                                                           =MMMMMMM         MMM    M    MMD
MMM M   M          MM.                   MMMMMM                    MMMMMMM  ..                           ZMMMMMMMMMMMM        MM    M    MMM
MMM M   MM        MMMM                 MMMMM,                      MMMMMMMM..MM                       MMMMMMM8    MMMMMMMM   OMM    M    MMN
MMM?M?   .M.     MMMMM               MMMMMMM                            MMMM         ,ZMM         +MMMMMMMD      MMMMMMMMMM  MMM    M    MMZ
.MMM MM         MMMMMM              MMMMMMMM                 MMMMMMM     MMM                  ?MMMMMMMM=         MMMD       $MMM    +   MMM 
 MMMM .MM       MMMMMMM         MMZ   MMMMMMM             MMMMMMMMMMM   MMMM             .MMMMMMMMMMM           MMMM        MMM    M   :MMM 
  MMMM    MM,   MMMMMMMM     :M.          =MMMM                   =M   MMMM          MMMMMMMMMMMMMMM          8MMMMM        MM.   ~M   MMMM 
   MMMM        .MMM MMMMMM                  MMMMM   MM                 MM.      MMMMMMMMMMM      MMM        MMMMMMM$        .    MM   MMMM  
   .MMM.       MMMMMMMMMMMMM$                .MMMMMMMM.                   DMMMMMMMMMMM.         MMMM     MMMMMMMMM            DMM    MMMM   
    MMMM       MMMMMMM MMMMMMMMM               ~MMMM:               .MMMMMMMMMMMM              MMMMM.=MMMMMMMMMMM          .M~     MMMM.    
     MMM.      MMMMMM  MMMMMMMMMMMMMM.                     .IMMMMMMMMMMMMMMMMMM               MMMMMMMMMMMM  MMMM                $MMMMM      
     MMM.     .MMMMMM  MMM    MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM,      MMM             MMMMMMMMMMM    MMMM               M MMMMM       
     +MMM      MMMMM   MMM     7MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM             MMMM       MMMMMMMMMMMMM      MMMM                 MMMM.        
      MMM      MMMMM7  MMM     MMMM          MMMM         MMM               .MMM   MMMMMMMMMMMMMMMMM     MMMM                 MMMM          
     .MMM      MMMMMMMMMMM?    MMM           MMM          MMM                MMMMMMMMMMMMMMMM   MMM    IMMM.                IMMM7           
     .MMM      MMMMMMMMMMMMM   MMM           MMM          MMM            NMMMMMMMMMMMMMMMM.     MMM   MMMM.                 MMMO            
     .MMM      MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM.          MMMMMMMM                  MMMO             
     .MMM      MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                MMMMMM                  MMMM              
     .MMM      MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM   MMM               MMMMM                   MMMM               
     .MMM      :MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM       .MMM             MMMMM.                   MMMM                
     .MMM       MMM MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM             MMM8          MMMMM                     MMMM                 
     .MMM       MMMM MMMM.MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM  MMM               OMMM        MMMMM7                     MMMM                  
      MMM        MMM MMMM  MMMM   MMMMMMMMMMMMMMMM           MMM                MMMM    IMMMMM                      ?MMMM                   
      MMM         MMM MMM.  MMM     MMM       MMMM           MMM                 MMMM MMMMMM                       MMMM?                    
     .MMM         MMMMMMM.  ~MMM    MMMM       MMM           MMM                 MMMMMMMMM           +           MMMMM                      
     IMM$          .MMMMM    MMMM   .MMM       MMM.          MMM              MMMMMMMMM          MM      ?M    OMMMM                        
     DMM~            8MMMMMMMMMMM    MMMM      MMMM          MMMD       MMMMMMMMMMM           MM      8MD    DMMMMD                         
     MMM                MMMMMMMMMMMMMMMMMMM    MMMM     .   MMMMMMMMMMMMMMMMMM8            MM       MM     MMMMMM                           
     MMM.                     NMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM               MM7      +MM      MMMMMM                             
     MMM.                                 .      ,,,.     .                        ~MM       MM       MMMMMM                                
     MMM             MO                                                         MM        MM.      MMMMMMM                                  
    .MMM      M       .M                    ,+?I7$7I?=,                    ~MM        MM        MMMMMMM                                     
     MMM      M          MM                                            MMM        MM        ?MMMMMMM                                        
     MMM      .M           :MM                                 ZMMMMM       +MMZ        .MMMMMMMM.                                          
     MMM        MM               =MMMMMMMMMMMMMMMMD?                 OMMM7           .MMMMMMMM.                                             
    .MMM          .MM                                        +MMMM                MMMMMMMM.                                                 
     MMM             MMM                        . :MMMMMM .                    MMMMMMMD                                                     
     MMMM                .  .,,.     ..  .                                 .MMMMMMM                                                         
      MMMM                                                              MMMMMMMM.                                                           
      .MMMM                                                          MMMMMMMM                                                               
        MMMM                                                 .ZMMMMMMMMMMD                                                                  
         MMMMM                                          MMMMMMMMMMMMMM                                                                      
           MMMMMM                                ,MMMMMMMMMMMMMN                                                                            
             MMMMMMMM                     MMMMMMMMMMMMMM$.                                                                                  
                MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                                                                                          
                      .  ~+7O8DNMMMMN8$=  .                                                                                                 
                                                               
                                                               
                                                               
                               Problem?                                      */      
