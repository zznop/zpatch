Import('env')

objs = [
    env.Object('./src/main.c'),
    env.Object('./src/file.c'),
    env.Object('./src/assemble.c'),
]

zpatch = env.Program(
    source=objs,
    target='zpatch',
)

Return('zpatch')
