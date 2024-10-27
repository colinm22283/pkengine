#include <pkengine/engine.hpp>

void PKEngine::Engine::start() {
   while (!contexts.empty()) {
       bool recheck_contexts;
       do {
           recheck_contexts = false;

           auto before_itr = contexts.before_begin();
           for (auto itr = contexts.begin(); itr != contexts.end(); before_itr++, itr++) {
               if (itr->should_exit()) {
                   contexts.erase_after(before_itr);
                   recheck_contexts = true;
                   break;
               }
           }
       }
       while (recheck_contexts);

       for (Context & context : contexts) context.update();

       glfw_instance.poll_events();
   }
}