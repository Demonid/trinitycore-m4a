alter table calendar_invites change column statustime response_time int(10) not null default -1;
update calendar_invites set response_time=-1 where response_time=0;
