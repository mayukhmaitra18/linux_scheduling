#include <linux/list.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include "../kernel/sched/sched.h"

#define PENTER \
	printk(KERN_INFO "entering %s\n", __FUNCTION__)
#define PENTERID(id) \
	printk(KERN_INFO "entering %s, %d\n", __FUNCTION__, (id));

struct my_rq {
	struct sched_entity *se;
	struct list_head list;
} my_rq[CPU_LOAD_IDX_MAX];

static int my_rq_initialized = 0;

void init_my_sched_class(void)
{
	int i;
	PENTER;

	for(i = 0 ; i < CPU_LOAD_IDX_MAX ; i++)
	{
		//todo : initialize my_rq[i]
		INIT_LIST_HEAD(&my_rq[i].list);
		my_rq[i].se = NULL;
		printk("%d initialized \n", i);

	}
	my_rq_initialized = 1;//initialized finished
}

static struct task_struct * my_pick_next_task(struct rq *rq, struct task_struct *prev, struct pin_cookie cookie)
{
	struct task_struct *var;
	struct my_rq *temp;

	if(!my_rq_initialized) return NULL;
	//todo- checking if my_rq[rq->cpu].list is empty or not, else getting the sched_entity of the first list element
	if(list_empty(&my_rq[rq->cpu].list)) return NULL;
	else
	{
		var1 = list_first_entry(&my_rq[rq->cpu].list, struct my_rq, list);
		var = container_of(var1->se, struct task_struct, se);
		printk(KERN_INFO "picking %s, prev: %d, pick: %d\n", __FUNCTION__, prev->pid, var->pid);
		return t;
	}
}

static void my_enqueue_task(struct rq *rq, struct task_struct *p, int flags)
{
	struct my_rq *var1;
	if(p == NULL || rq == NULL)
	{
		printk("p or rq null\n");
		return;
	}

	PENTERID(p->pid);
	//todo - allocating my_rq, initializing it with p->se and adding to my_rq[rq->cpu], increasing rq's running count
	var1 = kmalloc(sizeof(struct my_rq), GFP_KERNEL);
	if(var1 == NULL)
	{printk("error  null\n");
	return;}
	var1->se = &p->se;
	INIT_LIST_HEAD(&var1->list);
	list_add_tail(&var1->list, &my_rq[rq->cpu].list);
	add_nr_running(rq, 1);

}

static void my_dequeue_task(struct rq *rq, struct task_struct *p, int flags)
{

	struct list_head *var;
	PENTERID(p->pid);
	//todo - finding and deleting my_rq with p->se from my_rq[rq->cpu].list
	if(!list_empty(&my_rq[rq->cpu].list))
		list_for_each(var, &my_rq[rq->cpu].list)
		{
			struct my_rq *var1;
			var1 = list_entry(var, struct my_rq, list);
			if(var1->se == &p->se)
			{
				list_del(&var1->list);
				kfree(var1);
				sub_nr_running(rq, 1);
				return;
			}
		}
}

int my_prio(void) {return 1000;}

int is_my_prio(int prio) { return prio == my_prio() ? 1 : 0; }

int my_policy(void) { return 7; } // used 7 for sched_setscheduler

#ifdef CONFIG_SMP
static int my_select_task_rq(struct task_struct *p, int cpu, int sd_flag, int flags)
{
	PENTER;
	return task_cpu(p);
}
#endif

static void my_check_preempt_curr(struct rq *rq, struct task_struct *p, int flags)
{ PENTER; }

static void my_yield_task(struct rq *rq)
{ PENTER; }

static void my_put_prev_task(struct rq *rq, struct task_struct *prev)
{ PENTERID(prev->pid); }

static void my_set_curr_task(struct rq *rq)
{ PENTER; }

static void my_task_tick(struct rq *rq, struct task_struct *curr, int queued)
{ PENTER; }

static void my_switched_to(struct rq *rq, struct task_struct *p)
{ PENTERID(p->pid); }

static void my_prio_changed(struct rq *rq, struct task_struct *p, int oldprio)
{ PENTER; }

static unsigned int my_get_rr_interval(struct rq *rq, struct task_struct *task)
{ PENTER; return 0; }

static void my_update_curr(struct rq *rq)
{ PENTER; }


const struct sched_class my_sched_class = {
	.next			= &fair_sched_class,
	.enqueue_task		= my_enqueue_task,
	.dequeue_task		= my_dequeue_task,
	.yield_task		= my_yield_task,
	.check_preempt_curr	= my_check_preempt_curr,
	.pick_next_task		= my_pick_next_task,
	.put_prev_task		= my_put_prev_task,
	.set_curr_task		= my_set_curr_task,
#ifdef CONFIG_SMP
	.select_task_rq		= my_select_task_rq,
	.set_cpus_allowed	= set_cpus_allowed_common,
#endif
	.task_tick		= my_task_tick,
	.switched_to		= my_switched_to,
	.prio_changed		= my_prio_changed,
	.get_rr_interval	= my_get_rr_interval,
	.update_curr		= my_update_curr,
};
