import { FC } from 'react';

import { CircularProgress, Box, Typography, Theme } from '@mui/material';

interface LoadingSpinnerProps {
  height?: number | string;
}

const LoadingSpinner: FC<LoadingSpinnerProps> = ({}) => (
  <div>
    Loading
  </div>
);

export default LoadingSpinner;
