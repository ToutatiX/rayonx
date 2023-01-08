# Copyright (c) 2014-present PlatformIO <contact@platformio.org>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import sys
from traceback import format_exc

import click

from platformio import __version__, exception, maintenance
from platformio.cli import PlatformioCLI
from platformio.compat import IS_CYGWIN, ensure_python3


@click.command(
    cls=PlatformioCLI, context_settings=dict(help_option_names=["-h", "--help"])
)
@click.version_option(__version__, prog_name="PlatformIO Core")
@click.option("--force", "-f", is_flag=True, help="DEPRECATED", hidden=True)
@click.option("--caller", "-c", help="Caller ID (service)")
@click.option("--no-ansi", is_flag=True, help="Do not print ANSI control characters")
@click.pass_context

def cli(ctx, force, caller, no_ansi):  # pylint: disable=unused-argument
    try:
        if (
            no_ansi
            or str(
                os.getenv("PLATFORMIO_NO_ANSI", os.getenv("PLATFORMIO_DISABLE_COLOR"))
            ).lower()
            == "true"
        ):
            # pylint: disable=protected-access
            click._compat.isatty = lambda stream: False
        elif (
            str(
                os.getenv("PLATFORMIO_FORCE_ANSI", os.getenv("PLATFORMIO_FORCE_COLOR"))
            ).lower()
            == "true"
        ):
            click._compat.isatty = lambda stream: True
    except:  # pylint: disable=bare-except
        pass

    return maintenance.on_platformio_start(ctx, caller)


def main(argv):
    assert isinstance(argv, list)
    sys.argv = argv

    ensure_python3(raise_exception=True)
    cli()
    print("test")
    return "test"
    #return type(maintenance.on_platformio_start(argv, argv))


if __name__ == "__main__":
    print(main(['/Users/eliottsarrey/.julia/conda/3/bin/pio', 'device', 'list']))
