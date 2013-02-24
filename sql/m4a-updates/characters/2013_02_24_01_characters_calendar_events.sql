alter table calendar_events change column eventtime event_time int(10) not null default -1, change column time2 lockout_time int(10) not null default -1;
update calendar_events set event_time=-1 where event_time=0;
update calendar_events set lockout_time=-1 where lockout_time=0;
