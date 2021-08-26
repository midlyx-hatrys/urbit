import { map, pick } from 'lodash-es';
import React, { FunctionComponent, useEffect } from 'react';
import { Route, RouteComponentProps } from 'react-router-dom';
import { MenuState, Nav } from '../nav/Nav';
import useDocketState from '../state/docket';
import { RemoveApp } from '../tiles/RemoveApp';
import { SuspendApp } from '../tiles/SuspendApp';
import { Tile } from '../tiles/Tile';

type GridProps = RouteComponentProps<{
  menu?: MenuState;
}>;

export const Grid: FunctionComponent<GridProps> = ({ match }) => {
  const { charges, fetchCharges } = useDocketState((s) => pick(s, ['charges', 'fetchCharges']));
  const chargesLoaded = Object.keys(charges).length > 0;

  useEffect(() => {
    fetchCharges();
  }, []);

  return (
    <div className="flex flex-col">
      <header className="sticky top-0 left-0 z-30 flex justify-center w-full bg-white">
        <Nav menu={match.params.menu} />
      </header>

      <main className="h-full w-full flex justify-center pt-24 pb-32 relative z-0">
        {!chargesLoaded && <span>Loading...</span>}
        {chargesLoaded && (
          <div className="grid grid-cols-2 sm:grid-cols-3 md:grid-cols-4 gap-6 px-4 md:px-8 w-full max-w-6xl">
            {charges &&
              map(charges, (charge, desk) => <Tile key={desk} docket={charge} desk={desk} />)}
          </div>
        )}
        <Route exact path="/app/:desk/suspend">
          <SuspendApp />
        </Route>
        <Route exact path="/app/:desk/remove">
          <RemoveApp />
        </Route>
      </main>
    </div>
  );
};