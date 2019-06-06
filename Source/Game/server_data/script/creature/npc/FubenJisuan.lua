
JingYanLingQu_FB = 0  --�㲥���ƣ�2����һˢ�¡����Systemact

function n_FubenJisuan(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local LastInstance = role.GetRoleScriptData(RoleID, 1, RoleDataType["Last_Instance"])
	local FreeSize = role.GetBagFreeSize(RoleID)

    --�����ȡ�������˳���6�������ܼ�����ȡ
    local lingqu_num = 0
	if MapID == 3084407503 then --����ħ��m40
		lingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 10)
	elseif MapID == 3084406991 then--ͨڤ����m42
	    lingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 12)
	elseif MapID == 3084407759 then--��Դ�ؾ�m41
		lingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 16)
	elseif MapID == 3084408783 then--�������m45
	    lingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 15)
	elseif MapID == 3084407247 then--��ܭ�ع�m43
		lingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 14)
	elseif MapID == 3084408271 then--�Ϲ��޻�m47
		lingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 2)
	end

	if LastInstance == TargetID + InstanceID or lingqu_num >= 6 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 1779)		--������ȡ�����θ�����ɽ�����
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	elseif FreeSize<=1+Duanwu_On then

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 168)		--���ҿռ䲻��
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.DispatchRoleMsgEvent(RoleID, MsgID)

	else

		role.SetRoleScriptData(RoleID, 1, RoleDataType["Last_Instance"], TargetID + InstanceID)

		local Exp_table = {}
		Exp_table[50]=72238
		Exp_table[51]=78806
		Exp_table[52]=85773
		Exp_table[53]=94000
		Exp_table[54]=102227
		Exp_table[55]=110453
		Exp_table[56]=118680
		Exp_table[57]=127565
		Exp_table[58]=136000
		Exp_table[59]=144435
		Exp_table[60]=152870
		Exp_table[61]=161306
		Exp_table[62]=170137
		Exp_table[63]=176800
		Exp_table[64]=183463
		Exp_table[65]=190126
		Exp_table[66]=196789
		Exp_table[67]=203763
		Exp_table[68]=210500
		Exp_table[69]=217237
		Exp_table[70]=223974
		Exp_table[71]=230710
		Exp_table[72]=237107
		Exp_table[73]=243598
		Exp_table[74]=250089
		Exp_table[75]=256580
		Exp_table[76]=263071
		Exp_table[77]=269556
		Exp_table[78]=282205
		Exp_table[79]=294854
		Exp_table[80]=307502
		Exp_table[81]=320151
		Exp_table[82]=332800
		Exp_table[83]=351202
		Exp_table[84]=369604
		Exp_table[85]=388006
		Exp_table[86]=406408
		Exp_table[87]=424810
		Exp_table[88]=449364
		Exp_table[89]=473918
		Exp_table[90]=498473
		Exp_table[91]=523027
		Exp_table[92]=547584
		Exp_table[93]=574654
		Exp_table[94]=601724
		Exp_table[95]=628794
		Exp_table[96]=655864
		Exp_table[97]=682934
		Exp_table[98]=710003
		Exp_table[99]=737073
		Exp_table[100]=764143

		local Fuben_Level = {}
		Fuben_Level[3084407503] = {level=65, monster_num=120, boss_num=4, mon_pos=11, boss_pos=8}	--����ħ��m40
		Fuben_Level[3084406991] = {level=75, monster_num=120, boss_num=4, mon_pos=10, boss_pos=11}	--ͨڤ����m42
		Fuben_Level[3084407759] = {level=85, monster_num=150, boss_num=4, mon_pos=12, boss_pos=13}	--��Դ�ؾ�m41
		Fuben_Level[3084408783] = {level=95, monster_num=150, boss_num=4, mon_pos=14, boss_pos=4}	--�������m45
		Fuben_Level[3084407247] = {level=105, monster_num=180, boss_num=2, mon_pos=9, boss_pos=10}	--��ܭ�ع�m43
		Fuben_Level[3084408271] = {level=105, monster_num=90, boss_num=3, mon_pos=0, boss_pos=1}	--��ܭ�ع�m43

		local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		local Temp = 0.5
		local Team_Num = 1
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)

		if TeamID~=4294967295 then
			local Role = {}
			Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)

			for i=1, 6 do
				if Role[i] and Role[i]~= 4294967295 and Role[i]~= RoleID then
					Temp = Temp+0.1
					Team_Num = Team_Num+1
				end
			end
		end

		local Xiaoguai_num = map.GetMapScriptData(MapID, InstanceID, 1, Fuben_Level[MapID].mon_pos)
		local Boss_num = map.GetMapScriptData(MapID, InstanceID, 1, Fuben_Level[MapID].boss_pos)

		local Boss1 = Boss_num % 2
		local Boss2 = math.floor(Boss_num/2) % 2
		local Boss3 = math.floor(Boss_num/4) % 2
		local Boss4 = math.floor(Boss_num/8) % 2
		local Boss5 = math.floor(Boss_num/16) % 2
		local Boss6 = math.floor(Boss_num/32) % 2

		local Boss_sum = Boss1+Boss2+Boss3+Boss4

		if MapID == 3084407247 then
			Boss_sum = math.max(Boss1+Boss2, Boss3+Boss4, Boss5+Boss6)
		end

		if MapID == 3084408271 then
			Boss_sum = Boss_num
		end

		local Complete_Rate = math.min(Xiaoguai_num,Fuben_Level[MapID].monster_num)/Fuben_Level[MapID].monster_num * 0.75 + math.min(Boss_sum,Fuben_Level[MapID].boss_num)/Fuben_Level[MapID].boss_num * 0.25

		local level_dec = 1

		if RoleLevel>Fuben_Level[MapID].level then
			level_dec = 1-math.min(RoleLevel-Fuben_Level[MapID].level, 5)*0.15
		end

		if MapID == 3084408271 then
			Complete_Rate = Complete_Rate * 2.5
		end

		if Duanwu_On == 1 then
			Complete_Rate = Complete_Rate * 1.5
		end

		role.AddRoleExp(MapID, InstanceID, RoleID, Exp_table[RoleLevel]*Temp*Complete_Rate*level_dec)
		role.AddRoleItem(MapID, InstanceID, RoleID, 3304149, 2, -1, 8, 420)

		if Duanwu_On == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 3307)		--������ڼ�����ñ�а���ӽ���
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end

	    --��¼�������Ѿ���ȡ�˽���������+1
    	if MapID == 3084407503 then --����ħ��m40
		    local itemnum = math.floor(8*Complete_Rate*level_dec)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3302101, itemnum, -1, 8, 420)
			local jingyanlingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 10)+1
			map.SetMapScriptData(MapID, InstanceID, 1, 10, jingyanlingqu_num)
		elseif MapID == 3084406991 then--ͨڤ����m42
		    local itemnum = math.floor(10*Complete_Rate*level_dec)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3302101, itemnum, -1, 8, 420)
			local jingyanlingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 12)+1
			map.SetMapScriptData(MapID, InstanceID, 1, 12, jingyanlingqu_num)
		elseif MapID == 3084407759 then--��Դ�ؾ�m41
		    local itemnum = math.floor(12*Complete_Rate*level_dec)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3302101, itemnum, -1, 8, 420)
		    local jingyanlingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 16)+1
			map.SetMapScriptData(MapID, InstanceID, 1, 16, jingyanlingqu_num)
		elseif MapID == 3084408783 then--�������m45
		    local itemnum = math.floor(14*Complete_Rate*level_dec)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3302101, itemnum, -1, 8, 420)
			local jingyanlingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 15)+1
			map.SetMapScriptData(MapID, InstanceID, 1, 15, jingyanlingqu_num)
		elseif MapID == 3084407247 then--��ܭ�ع�m43
		    local itemnum = math.floor(14*Complete_Rate*level_dec)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3302101, itemnum, -1, 8, 420)
		    local jingyanlingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 14)+1
			map.SetMapScriptData(MapID, InstanceID, 1, 14, jingyanlingqu_num)
		elseif MapID == 3084408271 then--�Ϲ��޻�m47
		    local itemnum = math.floor(14*Complete_Rate*level_dec)
			role.AddRoleItem(MapID, InstanceID, RoleID, 3302101, itemnum, -1, 8, 420)
		    local jingyanlingqu_num = map.GetMapScriptData(MapID, InstanceID, 1, 2)+1
			map.SetMapScriptData(MapID, InstanceID, 1, 2, jingyanlingqu_num)
		end

		--6%�ļ��ʲ���ȫ���������������Ƶ���㲥��4%�ļ��ʲ���ȫ��С������ƹ㲥����ģ�����ι㲥�����С���Ϊ10����
		local rand = math.random(1,100)
		if rand <= 10 and JingYanLingQu_FB == 0 then
		    JingYanLingQu_FB = 1
			local MsgID = msg.BeginMsgEvent()
			--[[if rand<3 then	--�����ڣ��������ƣ��е�һ��ԡѪ��ս����ң�������ƣ������ڣ�NPC���ƣ�����ȡ�ˣ�����ֵ���㾭��Ķ��⽱����
				msg.AddMsgEvent(MsgID, 102, 100113)
				msg.AddMsgEvent(MsgID, 6, MapID)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 5, TargetTypeID)
				msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Temp*Complete_Rate*level_dec)
			elseif rand<5 then	--��ң�������ƣ��ڣ�NPC���ƣ�����ȡ�ˣ�����ֵ���㾭��ĸ������⽱����
				msg.AddMsgEvent(MsgID, 102, 100114)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 5, TargetTypeID)
				msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Temp*Complete_Rate*level_dec)
			elseif rand<7 then	--��ϲ��ң�������ƣ�������ܼƣ�����ֵ���㾭��ĸ���������
				msg.AddMsgEvent(MsgID, 102, 100115)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Temp*Complete_Rate*level_dec)
			else]] 	--���������š���˵��ҡ���������ƣ����ڡ���NPC���ƣ���������ȡ�ˣ�����ֵ���㾭��ĸ������齱����
				msg.AddMsgEvent(MsgID, 102, 100116)
				msg.AddMsgEvent(MsgID, 2, RoleID)
				msg.AddMsgEvent(MsgID, 5, TargetTypeID)
				msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Temp*Complete_Rate*level_dec)
			--end
			msg.DispatchBroadcast(MsgID, -1, -1, -1)
		end

		if Team_Num==6 and level_dec==1 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1780)
			--�����λ�ɱ��Ӣ��������ΪXX/XX����ɱ��������ΪXX/XX��
			--���ɻ�õľ��齱��ΪXXXX�㡣��������������Ѵ�6�ˣ����ȫ��齱��XXXX�㡣
			msg.AddMsgEvent(MsgID, 9, Xiaoguai_num)
			msg.AddMsgEvent(MsgID, 9, Fuben_Level[MapID].monster_num)
			msg.AddMsgEvent(MsgID, 9, Boss_sum)
			msg.AddMsgEvent(MsgID, 9, Fuben_Level[MapID].boss_num)
			msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Complete_Rate*level_dec)
			msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Temp*Complete_Rate*level_dec)
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif Team_Num~=6 and level_dec==1 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1781)
			--�����λ�ɱ��Ӣ��������ΪXX/XX����ɱ��������ΪXX/XX��
			--���ɻ�õľ��齱��ΪXXXX�㡣�����������������6�ˣ���ò��־��齱��XXXX�㡣
			msg.AddMsgEvent(MsgID, 9, Xiaoguai_num)
			msg.AddMsgEvent(MsgID, 9, Fuben_Level[MapID].monster_num)
			msg.AddMsgEvent(MsgID, 9, Boss_sum)
			msg.AddMsgEvent(MsgID, 9, Fuben_Level[MapID].boss_num)
			msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Complete_Rate*level_dec)
			msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Temp*Complete_Rate*level_dec)
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif Team_Num==6 and level_dec~=1 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1783)
			--�����λ�ɱ��Ӣ��������ΪXX/XX����ɱ��������ΪXX/XX��
			--���ɻ�õľ��齱��ΪXXXX�㡣��������������Ѵ�6�ˣ����ȫ��齱��XXXX�㡣
			--�����ĵȼ��ѳ����ø�������ȼ����ޣ����ܵ��ľ��齱�����յ�˥������
			msg.AddMsgEvent(MsgID, 9, Xiaoguai_num)
			msg.AddMsgEvent(MsgID, 9, Fuben_Level[MapID].monster_num)
			msg.AddMsgEvent(MsgID, 9, Boss_sum)
			msg.AddMsgEvent(MsgID, 9, Fuben_Level[MapID].boss_num)
			msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Complete_Rate*level_dec)
			msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Temp*Complete_Rate*level_dec)
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		elseif Team_Num~=6 and level_dec~=1 then

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 1784)
			--�����λ�ɱ��Ӣ��������ΪXX/XX����ɱ��������ΪXX/XX��
			--���ɻ�õľ��齱��ΪXXXX�㡣�����������������6�ˣ���ò��־��齱��XXXX�㡣
			--�����ĵȼ��ѳ����ø�������ȼ����ޣ����ܵ��ľ��齱�����յ�˥������
			msg.AddMsgEvent(MsgID, 9, Xiaoguai_num)
			msg.AddMsgEvent(MsgID, 9, Fuben_Level[MapID].monster_num)
			msg.AddMsgEvent(MsgID, 9, Boss_sum)
			msg.AddMsgEvent(MsgID, 9, Fuben_Level[MapID].boss_num)
			msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Complete_Rate*level_dec)
			msg.AddMsgEvent(MsgID, 9, Exp_table[RoleLevel]*Temp*Complete_Rate*level_dec)
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)

		end

	end

end

-- ע��
aux.RegisterCreatureEvent(3075221, 7, "n_FubenJisuan")
