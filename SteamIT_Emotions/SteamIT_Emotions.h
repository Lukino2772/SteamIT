#ifndef STEAMIT_EMOTIONS_H
#define STEAMIT_EMOTIONS_H
#include <Arduino.h>
#include <iostream>
using namespace std;

enum Emotion{
  Neutral,
  Happy,
  Sad,
  Angry,
  Scared
};

class SteamIT_Emotions {
    private:
      Emotion emotion;
      void setEmotion(Emotion e);
    public:
      SteamIT_Emotions();
      ~SteamIT_Emotions();

      void updateEmotions(string stimulus);
      Emotion getEmotion();
      string getEmotionName();

};

#endif
