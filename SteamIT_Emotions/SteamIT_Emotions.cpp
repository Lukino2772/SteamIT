#include "SteamIT_Emotions.h"

SteamIT_Emotions::SteamIT_Emotions(){
    currentEmotion = Neutral;
}
SteamIT_Emotions::~SteamIT_Emotions(){}
void SteamIT_Emotions::update(){
  stimulus.toLowerCase();
  if(stimulus == "praise"){
    setEmotion(Happy);
  }else if (stimulus == "hit"){
    setEmotion(Angry);
  }else if (stimulus == "ignore"){
    setEmotion(Sad);
  }else if (stimulus == "scare"){
    setEmotion(Scare);
  } else {
    setEmotion(Neutral);
  }
}
void SteamIT_Emotions::setEmotion(Emotion e){
  currentEmotion = e;
}
Emotion SteamIT_Emotions::getEmotion(){
  return currentEmotion;
}
string SteamIT_Emotions::getEmotionName(){
  switch(currentEmotion){
    case Happy: return "Happy";
    case Sad: return "Sad";
    case Angry: return "Angry";
    case Scare: return "Scare";
    case Neutral:
    default: return "Neutral";
  }
}