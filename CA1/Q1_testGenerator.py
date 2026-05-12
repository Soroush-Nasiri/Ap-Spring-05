import random
import argparse
import os
import subprocess
from typing import Optional


WORDS = [
    "Alpha","Beta","Gamma","Delta","Epsilon","Zeta","Eta","Theta",
    "Build","Test","Deploy","Review","Design","Compile","Link","Run",
    "Parse","Scan","Fetch","Push","Pull","Merge","Commit","Check",
    "Init","Setup","Clean","Format","Lint","Pack","Release","Archive",
    "Load","Cache","Flush","Sync","Verify","Validate","Generate","Render",
]

def rword():
    return random.choice(WORDS) + str(random.randint(1, 99))

def rlist():
    return random.choice(["Proj","Sprint","Build","Phase","Task"]) + str(random.randint(1, 20))

class Sim:
    def __init__(self):
        self.tasks = {}

    def reset(self):
        self.tasks = {}

    def existing(self):
        return list(self.tasks.keys())

    def pending(self):
        return [i for i, t in self.tasks.items() if not t['done']]

    def done_ids(self):
        return [i for i, t in self.tasks.items() if t['done']]

    def ready(self):
        out = []
        for i, t in self.tasks.items():
            if t['done']: continue
            if all(self.tasks.get(d, {}).get('done', False) for d in t['deps']):
                out.append(i)
        return out

    def blocked(self):
        out = []
        for i, t in self.tasks.items():
            if t['done']: continue
            if not all(self.tasks.get(d, {}).get('done', False) for d in t['deps']):
                out.append(i)
        return out

    def has_dependents(self, tid):
        return any(tid in t['deps'] for t in self.tasks.values())

    def non_removable(self):
        return [i for i in self.tasks if self.has_dependents(i)]

    def fresh_id(self, avoid=()):
        c = random.randint(1, 9999)
        while c in self.tasks or c in avoid:
            c = random.randint(1, 9999)
        return c

    def ghost_id(self):
        c = random.randint(1, 9999)
        while c in self.tasks:
            c = random.randint(1, 9999)
        return c

    def add(self, tid, title, deps=()):
        self.tasks[tid] = {'title': title, 'done': False, 'deps': sorted(deps)}

    def mark_done(self, tid):
        self.tasks[tid]['done'] = True

    def remove(self, tid):
        del self.tasks[tid]

def gen_dup(sim: Sim, lines: list):
    existing = sim.existing()
    if existing:
        dup_id = random.choice(existing)
        lines.append(f"ADD {dup_id} {rword()}")
    else:
        tid = sim.fresh_id()
        title = rword()
        lines.append(f"ADD {tid} {title}")
        sim.add(tid, title)
        lines.append(f"ADD {tid} {rword()}")


def gen_notfound(sim: Sim, lines: list):
    ghost = sim.ghost_id()
    cmd = random.choice(['DONE', 'STATUS', 'REMOVE'])
    lines.append(f"{cmd} {ghost}")


def gen_depnotfound(sim: Sim, lines: list):
    ghost = sim.ghost_id()
    tid = sim.fresh_id()
    title = rword()
    existing = sim.existing()

    if existing and random.random() < 0.5:
        valid_dep = random.choice(existing)
        if random.random() < 0.5:
            deps_str = f"{ghost} {valid_dep}"
        else:
            deps_str = f"{valid_dep} {ghost}"
    else:
        deps_str = str(ghost)

    lines.append(f"ADD_DEP {tid} {title} {deps_str}")


def gen_prereq(sim: Sim, lines: list):
    blocked = sim.blocked()
    if blocked:
        tid = random.choice(blocked)
        lines.append(f"DONE {tid}")
    else:
        if not sim.existing():
            dep_id = sim.fresh_id()
            dep_title = rword()
            lines.append(f"ADD {dep_id} {dep_title}")
            sim.add(dep_id, dep_title)

        dep_ids = [random.choice(sim.pending() or sim.existing())]
        tid = sim.fresh_id()
        title = rword()
        dep_str = ' '.join(map(str, dep_ids))
        lines.append(f"ADD_DEP {tid} {title} {dep_str}")
        sim.add(tid, title, dep_ids)
        lines.append(f"DONE {tid}")


def gen_hasdep(sim: Sim, lines: list):
    non_rm = sim.non_removable()
    if non_rm:
        tid = random.choice(non_rm)
        lines.append(f"REMOVE {tid}")
    else:
        p_id = sim.fresh_id()
        p_title = rword()
        lines.append(f"ADD {p_id} {p_title}")
        sim.add(p_id, p_title)

        c_id = sim.fresh_id()
        c_title = rword()
        lines.append(f"ADD_DEP {c_id} {c_title} {p_id}")
        sim.add(c_id, c_title, [p_id])

        lines.append(f"REMOVE {p_id}")

def gen_valid_add(sim: Sim, lines: list):
    tid = sim.fresh_id()
    title = rword()
    lines.append(f"ADD {tid} {title}")
    sim.add(tid, title)


def gen_valid_add_dep(sim: Sim, lines: list):
    existing = sim.existing()
    if not existing:
        gen_valid_add(sim, lines)
        return
    n = random.randint(1, min(2, len(existing)))
    deps = random.sample(existing, n)
    tid = sim.fresh_id()
    title = rword()
    lines.append(f"ADD_DEP {tid} {title} " + ' '.join(map(str, deps)))
    sim.add(tid, title, deps)


def gen_valid_done(sim: Sim, lines: list):
    ready = sim.ready()
    if ready:
        tid = random.choice(ready)
        lines.append(f"DONE {tid}")
        sim.mark_done(tid)


def gen_valid_remove(sim: Sim, lines: list):
    removable = [i for i in sim.existing() if not sim.has_dependents(i)]
    if removable:
        tid = random.choice(removable)
        lines.append(f"REMOVE {tid}")
        sim.remove(tid)


ERROR_GENERATORS = {
    'dup':         gen_dup,
    'notfound':    gen_notfound,
    'depnotfound': gen_depnotfound,
    'prereq':      gen_prereq,
    'hasdep':      gen_hasdep,
}

VALID_FILLERS = [gen_valid_add, gen_valid_add_dep, gen_valid_done, gen_valid_remove]


def build_test(error_type: str, num_errors: int, padding: int) -> str:
    sim = Sim()
    lines = []

    name = rlist()
    lines.append(f"NEW_LIST {name}")
    sim.reset()

    seed_count = random.randint(2, 5)
    for _ in range(seed_count):
        gen_valid_add(sim, lines)

    schedule = []
    for _ in range(num_errors):
        schedule.append('error')
    for _ in range(padding):
        schedule.append('valid')
    random.shuffle(schedule)

    err_types = list(ERROR_GENERATORS.keys())

    for slot in schedule:
        if slot == 'error':
            etype = random.choice(err_types) if error_type == 'all' else error_type
            ERROR_GENERATORS[etype](sim, lines)
        else:
            filler = random.choice(VALID_FILLERS)
            filler(sim, lines)

    lines.append("EXIT")
    return '\n'.join(lines) + '\n'


def get_output(inp: str, binary: str) -> Optional[str]:
    try:
        r = subprocess.run([binary], input=inp, capture_output=True, text=True, timeout=5)
        return r.stdout
    except Exception:
        return None


def count_errors(output: str) -> int:
    error_prefixes = (
        "Error: task",
        "Error: dependency",
        "Cannot complete task",
        "Cannot remove task",
    )
    return sum(1 for line in output.splitlines()
               if any(line.startswith(p) for p in error_prefixes))

def main():
    parser = argparse.ArgumentParser(
        description='Error-focused test generator for CA1 TodoList',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument('-n', '--num', type=int, default=1,
                        help='Number of tests to generate (default: 1)')
    parser.add_argument('--error', default='all',
                        choices=list(ERROR_GENERATORS.keys()) + ['all'],
                        help='Which error type to focus on (default: all)')
    parser.add_argument('--errors-per-test', type=int, default=3,
                        help='Deliberate errors injected per test (default: 3)')
    parser.add_argument('--padding', type=int, default=6,
                        help='Valid commands between errors (default: 6)')
    parser.add_argument('-o', '--outdir', default=None,
                        help='Output directory (default: stdout)')
    parser.add_argument('--seed', type=int, default=None,
                        help='Random seed for reproducibility')
    parser.add_argument('--solution', default='./todolist',
                        help='Path to compiled solution binary (default: ./todolist)')
    parser.add_argument('--no-output', action='store_true',
                        help="Don't run solution; only produce inputs")
    parser.add_argument('--list-types', action='store_true',
                        help='Print available error types and exit')
    args = parser.parse_args()

    if args.list_types:
        print("Available error types:")
        descs = {
            'dup':         'Error: task <id> already exists.',
            'notfound':    'Error: task <id> not found.',
            'depnotfound': 'Error: dependency <id> not found.',
            'prereq':      'Cannot complete task <id>: prerequisites not met.',
            'hasdep':      'Cannot remove task <id>: other tasks depend on it.',
            'all':         'Random mix of all the above.',
        }
        for k, v in descs.items():
            print(f"  {k:<14}  →  {v}")
        return

    if args.seed is not None:
        random.seed(args.seed)

    if args.outdir:
        os.makedirs(args.outdir, exist_ok=True)

    for i in range(1, args.num + 1):
        test_input = build_test(args.error, args.errors_per_test, args.padding)

        expected = None
        if not args.no_output:
            expected = get_output(test_input, args.solution)

        if expected is not None and count_errors(expected) == 0:
            for _ in range(10):
                test_input = build_test(args.error, args.errors_per_test, args.padding)
                expected = get_output(test_input, args.solution)
                if expected and count_errors(expected) > 0:
                    break

        n_err = count_errors(expected) if expected else '?'

        if args.outdir:
            in_path  = os.path.join(args.outdir, f"err{i:03d}.in")
            out_path = os.path.join(args.outdir, f"err{i:03d}.out")
            with open(in_path, 'w') as f:
                f.write(test_input)
            if expected is not None:
                with open(out_path, 'w') as f:
                    f.write(expected)
            print(f"  [{args.error:>12}]  err{i:03d}  —  {n_err} error line(s)")
        else:
            print(f"{'='*60}")
            print(f"TEST ERR-{i:03d}  [type: {args.error}]  ({n_err} error line(s))")
            print(f"{'='*60}")
            print("--- INPUT ---")
            print(test_input.rstrip())
            if expected is not None:
                print("--- EXPECTED OUTPUT ---")
                print(expected.rstrip() if expected.strip() else "(empty)")
            print()


if __name__ == '__main__':
    main()
    