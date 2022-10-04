#ifndef LOCK_FREE_AUDIO_QUEUE_H
#define LOCK_FREE_AUDIO_QUEUE_H

#include <atomic>
#include <memory>

/* 
 *  This lock free queue assumes that there is a single consumer.
 *  Multiple consumer would require reference counting that would make_shared
 *  this implementation more complex.
 * */

struct AudioPackage {
  uint8_t *m_data = nullptr;
  int m_len;

  AudioPackage(uint8_t *t_stream, int t_len) : m_len(t_len) {
     m_data = new uint8_t[m_len];

     for(int i = 0; i < m_len ; i++ ){
         m_data[i] =  t_stream[i];
     }
  }
};

class LockFreeAudioQueue {

public:

  // delete copy contructor
  LockFreeAudioQueue();
  LockFreeAudioQueue(const LockFreeAudioQueue &t_other) = delete;
  LockFreeAudioQueue& operator=(const LockFreeAudioQueue &t_other) = delete;

  ~LockFreeAudioQueue();

  void push(AudioPackage &&t_audio_package);
  std::shared_ptr<AudioPackage> pop();
  bool empty();
   
private:
  struct Node {
    std::shared_ptr<AudioPackage> m_data;
    Node* m_next = nullptr;
    Node():m_next(nullptr){};
  };

  std::atomic<Node*> m_head = nullptr;

  std::atomic<Node*> m_tail = nullptr;
  Node* pop_head();
};

#endif // !LOCK_FREE_AUDIO_QUEUE_H
