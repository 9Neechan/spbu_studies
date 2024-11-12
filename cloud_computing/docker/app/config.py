from dataclasses import dataclass
from environs import Env
from pydantic import PostgresDsn

@dataclass
class PGConfig:
    dsn: PostgresDsn
    is_echo: bool


@dataclass
class Config:
    pg_db: PGConfig


def load_config(path: str | None = None) -> Config:
    env = Env()
    env.read_env(path)
    return Config(
        pg_db=PGConfig(dsn=env('PG_DSN'), is_echo=bool(env('IS_ECHO'))),
    )