Import('env')

objs = [
    env.Object('./src/main.c'),
]

zpatch = env.Program(
    source=objs,
    target='zpatch',
)

Return('zpatch')
