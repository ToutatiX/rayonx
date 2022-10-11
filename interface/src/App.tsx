import React, { FC, RefObject } from 'react';
import { SnackbarProvider } from 'notistack';

import { FeaturesLoader } from './contexts/features';
import AppRouting from './AppRouting';

const App: FC = () => {
  	const notistackRef: RefObject<any> = React.createRef();

  	const onClickDismiss = (key: string | number | undefined) => () => {
		notistackRef.current.closeSnackbar(key);
  	};

	return (
		<main>
			<SnackbarProvider
				maxSnack={3}
				anchorOrigin={{ vertical: 'bottom', horizontal: 'left' }}
				ref={notistackRef}
				action={(key) => (
				<div onClick={onClickDismiss(key)} >
					Close
				</div>
				)}>
				<FeaturesLoader>
					<AppRouting />
				</FeaturesLoader>
			</SnackbarProvider>
		</main>
	);
};

export default App;
