#include "watchpoint.h"
#include "expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
static int cnt = 0;

void init_wp_pool() {
    int i;
    for (i = 0; i < NR_WP; i ++) {
        wp_pool[i].NO = i;
        wp_pool[i].next = &wp_pool[i + 1];
        wp_pool[i].info = -1;
    }
    wp_pool[NR_WP - 1].next = NULL;
    head = NULL;
    free_ = &wp_pool[0];
}

/* TODO: Implement the functionality of watchpoint */
void free_wp(int id,WP **wp)
{
    assert(id <= cnt);
    
    WP * removed_wp = NULL;
    WP * temp_head = head;
    WP * temp_free = free_;

    /* delete from head */
    if(id == 1){
        removed_wp = head;
        head = temp_head->next;
    }else{
        for(int i = 1; i < id-1; ++i) { temp_head = temp_head->next; }
        removed_wp = temp_head->next;
        temp_head->next = temp_head->next->next;
    }

    /* add removed element to free_ */
   if(temp_free->NO > removed_wp->NO){
       removed_wp->next = temp_free;
       temp_free = removed_wp;
   }else{
       while(temp_free->next){
           if(temp_free->next->NO > removed_wp->NO) break;
            temp_free = temp_free->next;
       }
       removed_wp->next = temp_free->next;
       temp_free->next = removed_wp;
   }
    --cnt;

    *wp = head;
    temp_head = head;
   for(int i = 0; i < cnt; ++i)
   {
       temp_head->NO  = i;
     temp_head = temp_head->next;
   }
}

WP* new_wp()
{
    assert(free_ != NULL);

    WP *oldfree_ = free_;
    if(cnt == NR_WP) {
        free_ = NULL;
    }else{
        free_ = free_->next;
    }

    WP *temp_head = head;

    if(head == NULL) {head = &wp_pool[0]; head->next = NULL;}
    else{
        if(temp_head->NO > oldfree_->NO){
            oldfree_->next = temp_head;
            head = oldfree_;
        }else{

            while(temp_head->next)
            {
                if(temp_head->next->NO >oldfree_->NO)  break;
                temp_head = temp_head->next;
            }

            oldfree_->next = temp_head->next;
            temp_head->next = oldfree_;
        }
    }
    oldfree_->NO = cnt;

    ++cnt;
    return oldfree_;
}

