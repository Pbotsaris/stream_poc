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
  uint8_t *m_data;
  int m_len;

  AudioPackage(uint8_t *t_stream, int t_len) : m_data(t_stream), m_len(t_len) {}
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
   
private:
  struct Node {
    std::shared_ptr<AudioPackage> m_data;
    Node* m_next;
    Node():m_next(nullptr){};
  };

  std::atomic<Node*> m_tail = nullptr;
  std::atomic<Node*> m_head = nullptr;

  Node* pop_head();
};

#endif // !LOCK_FREE_AUDIO_QUEUE_H
