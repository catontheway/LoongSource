--ϵͳ��ʱ�
--�Scriptdata 1 2 3 4 5 6 7 8 9 10 11 12�Ѿ�������ʹ��
--�Scriptdata 13 14 15�Ѿ���������ʹ��
--�Scriptdata 16�Ѿ��������ʹ��
--�Scriptdata 17�Ѿ�����֮Ѩ�ʹ��
--�Scriptdata 40 41�Ѿ�������������������ռ��
--�scriptdata 42 ���������Ϧ�Ϳ�ѧ�������

guangbo_baizhan = 0  --��¼��1�������Ƿ����˴�����ȫ���㲥
shixian_hecheng = 0  --��¼�ϳ����˵�id
shenyunfei_ID = 0
dragon_timer = 0
dragon_timerbegin = 0

systemactivity_timermin =0

function SystemActivity_OnTimerMin(actID)
    local curmon = tonumber(os.date("%m"))
	local curday = tonumber(os.date("%d"))
	local curmin = tonumber(os.date("%M"))
	local curhour = tonumber(os.date("%H"))
	local curweekday = tonumber(os.date("%w"))


	systemactivity_timermin= systemactivity_timermin+1

	--������ά������-��֮Ѩ

	if systemactivity_timermin==20 and LongZhiXue_Buchang == 1 then

		local bossindex = act.GetActScriptData(99, 1, 17)

		if bossindex==1 then
			map.MapCreateCreature(3017298383, -1, 1550201, 2054, 10964, 2544)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 3630)		--�����������ڷ������
			msg.DispatchWorldMsgEvent(MsgID)

		elseif bossindex==2 then
			map.MapCreateCreature(3017299663, -1, 1550202, 1166, 13219, 1873)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 3631)		--�����������ڷ������
			msg.DispatchWorldMsgEvent(MsgID)

		elseif bossindex==3 then
			map.MapCreateCreature(3017299919, -1, 1550203, 1803, 24427, 2568)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 3632)		--�����������ڷ������
			msg.DispatchWorldMsgEvent(MsgID)

		elseif bossindex==4 then
			map.MapCreateCreature(3017299151, -1, 1550204, 1915, 3100, 761)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 3633)		--�����������ڷ������
			msg.DispatchWorldMsgEvent(MsgID)

		end

	end

	--GMԣ��ɽ´�

	if GM_fubenhuodong_on == 1 then

		if curmin == GM_fubenhuodong_m and curhour == (GM_fubenhuodong_h+3)%24 then

			GM_fubenhuodong_on = 0

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1810)	--�·��֮����ѹ��ԣ��ɽ´���ѽ�����
			msg.DispatchWorldMsgEvent(MsgID)

		else
			if curmin == (GM_fubenhuodong_m+30)%60 then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 1809)	--�·��֮����ѹ��ԣ��ɽ´���ѿ�������x������������սԣ��ɽ´��������ɱ���조���а�ɽ��
				--����Ҿ��ɻ�����100000��ľ��齱����
				msg.AddMsgEvent(MsgID, 9, 30*(6-GM_fubenhuodong_t))
				GM_fubenhuodong_t=GM_fubenhuodong_t+1
				msg.DispatchWorldMsgEvent(MsgID)
			elseif curmin == GM_fubenhuodong_m and curhour ~= GM_fubenhuodong_h then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 1809)	--�·��֮����ѹ��ԣ��ɽ´���ѿ�������x������������սԣ��ɽ´��������ɱ���조���а�ɽ��
				--����Ҿ��ɻ�����100000��ľ��齱����
				msg.AddMsgEvent(MsgID, 9, 30*(6-GM_fubenhuodong_t))
				GM_fubenhuodong_t=GM_fubenhuodong_t+1
				msg.DispatchWorldMsgEvent(MsgID)
			end
		end
	end

	--GM�����̳�

	if GM_fubenhuodong2_on == 1 then

		if curmin == GM_fubenhuodong2_m and curhour == (GM_fubenhuodong2_h+3)%24 then

			GM_fubenhuodong2_on = 0

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1812)	--�·��֮����ѹ�������̳���ѽ�����
			msg.DispatchWorldMsgEvent(MsgID)

		else
			if curmin == (GM_fubenhuodong2_m+30)%60 then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 1811)	--�·��֮����ѹ��ԣ��ɽ´���ѿ�������x������������սԣ��ɽ´��������ɱ���조���а�ɽ��
				--����Ҿ��ɻ�����100000��ľ��齱����
				msg.AddMsgEvent(MsgID, 9, 30*(6-GM_fubenhuodong2_t))
				GM_fubenhuodong_t=GM_fubenhuodong2_t+1
				msg.DispatchWorldMsgEvent(MsgID)
			elseif curmin == GM_fubenhuodong2_m and curhour ~= GM_fubenhuodong2_h then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 1811)	--�·��֮����ѹ��ԣ��ɽ´���ѿ�������x������������սԣ��ɽ´��������ɱ���조���а�ɽ��
				--����Ҿ��ɻ�����100000��ľ��齱����
				msg.AddMsgEvent(MsgID, 9, 30*(6-GM_fubenhuodong2_t))
				GM_fubenhuodong_t=GM_fubenhuodong2_t+1
				msg.DispatchWorldMsgEvent(MsgID)
			end
		end
	end

	--GM��¹�񼣻

	if GM_bangpaihuodong_on == 1 then

		local temp_flag = 0

		if curmin == GM_bangpaihuodong_m and curhour == GM_bangpaihuodong_h then
			temp_flag=1
		end

		if temp_flag==0 and curmin==(GM_bangpaihuodong_m+30)%60 then

			GM_bangpaihuodong_on=0

			for i=1,20 do
				if GM_bangpaihuodong_rank[i].rank<=5 then
					guild.ModifyGuildMaterial(GM_bangpaihuodong_rank[i].GuildID, GM_bangpaihuodong_rank[i].lastkiller, 2000, 102)
				end
			end

			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 1814)	--���ɻ֮����ս����¹�񼣡��ѽ���������ǰ��İ����ѻ��2000�����ʲĽ�����
			msg.DispatchWorldMsgEvent(MsgID)

		end
	end

	--���������
	if Open_Arena == 1 and curmin == 30 and curhour % 2 == 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 2537)    --������ѣ��¶���������������ϵͳ�Ѿ����ţ���ҿ��Ե���¹�ġ�2071��2666�������������¶Ի����뾺������
		msg.DispatchWorldMsgEvent(MsgID)
	end

	--Ӣ������
	if PVPactivity_trigger == 1 then
		if day == "Monday" or day == "Friday" or day == "Wednesday" or day == "Sunday" then
			if curhour == 7 and curmin >= 30 and curmin % 10 == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100107)    --Ӣ������������<color=0xffff0000>��8��<color=0xfffff7e0>׼ʱ��ʼ��
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
		--[[elseif day == "Wednesday" or day == "Sunday" then
			if curhour == 7 and curmin >= 50 and curmin <= 59 then
			    if IsCreateAct18Instance == 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 2861)   --���δ��ʼ�����޷�ִ�иò���
					msg.DispatchMapMsgEvent(MsgID, 2983743695, 1, -1)
				end
				if IsCreateAct19Instance == 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 2861)   --���δ��ʼ�����޷�ִ�иò���
					msg.DispatchMapMsgEvent(MsgID, 2983743695, 2, -1)
				end
				if IsCreateAct20Instance == 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 2861)   --���δ��ʼ�����޷�ִ�иò���
					msg.DispatchMapMsgEvent(MsgID, 2983743695, 3, -1)
				end
				if IsCreateAct21Instance == 1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 101, 2861)   --���δ��ʼ�����޷�ִ�иò���
					msg.DispatchMapMsgEvent(MsgID, 2983743695, 4, -1)
				end
			end
			if curhour == 7 and curmin >= 30 and curmin % 10 == 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100110)    --Ⱥ����¹�������<color=0xffff0000>��8��<color=0xfffff7e0>׼ʱ��ʼ��������7��50֮ǰǰ��������
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end]]
		end
    end

	--��ս������

	if curweekday==6 and curhour==22 and curmin==30 then
		map.MapCreateColCreature(3695619387, -1, 4500705, 800, 10350, 590)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 101, 3028)    --������������������������ǡ�������������츮�������γ�������ǰ���ܷ⣡
		msg.DispatchWorldMsgEvent(MsgID)
	end

	--��ն���޺���־�ɳ�
    guangbo_baizhan = 0

	--����������ȡ�㲥��ÿ�����������һ��
	if curmin % 2 == 0 then
        JingYanLingQu_FB = 0
	end

	--[[���������ҹ㲥
	if curmin == 20 or curmin == 50 then
		if FeiSheng_trigger == 1 and act.GetActScriptData(99, 1, 5) ~= 0 then
			for i= 7,11 do
				if act.GetActScriptData(99, 1, i) ~= nil and act.GetActScriptData(99, 1, i) > 0 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 102, 100122)
					msg.AddMsgEvent(MsgID, 2, act.GetActScriptData(99, 1, i-6))
					msg.AddMsgEvent(MsgID, 9, i-6)
					msg.DispatchBroadcast(MsgID,-1,-1,-1)
					act.SetActScriptData(99, 1, i, act.GetActScriptData(99, 1, i)-1)
				    act.SaveActScriptData(99)
				end
			end
		end
	end]]

    --ÿ��������20��15ˢ��ʵ����װ���ϳ�����
	if (curweekday==5 and ((curhour==20 and curmin>=15) or curhour>=21)) or (curweekday==6 and ((curhour==20 and curmin<=15) or curhour<=19)) then
	    if shixian_hecheng == 0 then--��¼�ϳ����˵�id
			shixian_hecheng = map.MapCreateColCreature(3017298383, -1, 4900486, 991, 10000, 1985)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 2551)    --ʱ����װ���ϳ������Ѿ������ڷ���ġ�991,1985��������24Сʱ��ǰ���ϳɣ�
			msg.DispatchWorldMsgEvent(MsgID)
		end
	else
	    if shixian_hecheng ~= 0 then
		    map.MapDeleteCreature(3017298383, -1, shixian_hecheng)
			shixian_hecheng = 0
		end
	end

	--ÿ����Сʱˢ��12��������
	if curhour % 2 == 0 and curmin == 0 then
		if FeiSheng_trigger == 1 then          --and act.GetActScriptData(99, 1, 12) == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 3178)    --�������Ѿ����������أ���ɱ���ǿ��Ի�÷���ƾ֤�����������ף�
			msg.DispatchMapMsgEvent(MsgID, 3017299407, -1)
			--map.MapCreateColCreature(3017299407, -1, 1535326, feisheng_qilin[1].x, feisheng_qilin[1].y, feisheng_qilin[1].z, 1, "")
			for i= 2,11 do
				map.MapCreateColCreature(3017299407, -1, 1535327, feisheng_qilin[i].x, feisheng_qilin[i].y, feisheng_qilin[i].z, 1, "")
			end
		end
	end
	if (curhour == 11 or curhour == 20 or curhour == 1) and curmin == 30 then
		if FeiSheng_trigger == 1 then -- and act.GetActScriptData(99, 1, 12) == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100126)    --��������Ѿ����������أ���ɱ���ǿ��Ի������ǿ�����ߺͷ���ƾ֤�����������ף�
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
			--map.MapCreateColCreature(3017299407, -1, 1535326, feisheng_qilin[1].x, feisheng_qilin[1].y, feisheng_qilin[1].z, 1, "")
    	end
	end
	if (curhour == 12 or curhour == 21 or curhour == 2) and curmin == 0 then
		if FeiSheng_trigger == 1 then    --and act.GetActScriptData(99, 1, 12) == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100123)    --��������Ѿ����������أ���ɱ���ǿ��Ի������ǿ�����ߺͷ���ƾ֤�����������ף�
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
			map.MapCreateColCreature(3017299407, -1, 1535326, feisheng_qilin[1].x, feisheng_qilin[1].y, feisheng_qilin[1].z, 1, "")
    	end
	end

	--���˷�����ÿ����������鱦���
	if FeiSheng_trigger == 1 and act.GetActScriptData(99, 1, 1) ~= 0 then
	    if curweekday == 0 and curhour % 2 == 0 and curhour >= 6  and curhour <= 14 and curmin == 5 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 3132)    --��������һ��������ü�ɹٽ����������칬֮�����ֿ��鱦������ʱ����40�����ϵ���Ҷ�����ǰȥ������
			msg.DispatchWorldMsgEvent(MsgID)
		end
		if curweekday == 0 and ((curhour==13 or (curhour==14 and curmin <= 30)) or KuiLingBaoShu_Start == 1) then
		    KuiLingBaoShu_min = KuiLingBaoShu_min + 1
		    KuiLingBaoShu(curweekday, curhour, curmin, KuiLingBaoShu_min)
		end
	end


	--ÿ�������12:00,12:05,12:10,3:00��6:00�㲥������ȡ���ƿ�չ���ɻ��
	if (curhour == 12 and (curmin == 0 or curmin == 5 or curmin == 10)) or ((curhour == 15 or curhour == 18) and curmin == 0) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100127)    --���ɵİ���ÿ��12:00֮������ڷ����ս�����еĳ��񷢲��˴���ȡһ������˷������ʹ�ø����ƽ�����Ϊ�Լ����ɵĳ�Ա����һ����˷���İ�������������
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end

	if city.GetCityAppointedAtt(3, 2) ~= nil and city.GetCityAppointedAtt(3, 2) ~= 4294967295 then
	    --ÿ������0:00��������ȡ����������ʱ���һ��

        if curhour == 0 and curmin == 0 then
			if act.GetActScriptData(99, 1, 16) > 0 then
				act.SetActScriptData(99, 1, 16, act.GetActScriptData(99, 1, 16)-1)
   			end
		elseif (curhour == 18 or curhour == 19 or curhour == 20) and curmin == 5 then
		    if act.GetActScriptData(99, 1, 16) > 0 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 102, 100129)    --�������������ʩ���ݣ�֮����������18:00-21:00������ҿ���ǰ����ս�����ڳ��񷢲��˴���ȡ���齱����
				msg.DispatchBroadcast(MsgID,-1,-1,-1)
			end
        end

		--ʱ�䵽ˢ���ѵ�ħ����
		if curhour == 16 and curmin == 0 and curweekday == 6 then
		    map.MapCreateColCreature(3695619387, -1, 4900709, chengzhu_pos[1].x, chengzhu_pos[1].y, chengzhu_pos[1].z, 1, "")
	        local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100130)    --�ѵ�ħ��������������ս�飡������Ҷ����Ըϵ�����ս����������������XX��֮ǰȡ��ʤ���������й�ս����Ҷ����Դ��ѵ�ħ���ദ��ø߶�ľ��齱����
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
		elseif curhour == 16 and curmin % 10 == 0 and Liedimo_dead == 0 and curweekday == 6 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100131)    --�ѵ�ħ��������������ս�飡������Ҷ����Ըϵ�����ս����������������XX��֮ǰȡ��ʤ���������й�ս����Ҷ����Դ��ѵ�ħ���ദ��ø߶�ľ��齱����
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
		end
	end

	if (curmon == 7 and curday > 10) or (curmon == 8 and curday <= 2) then
		if ((curhour == 20 and curmin >= 30) or (curhour == 21 and curmin < 30)) and shenyunfei_ID == 0 then
		    shenyunfei_ID = map.MapCreateColCreature(3017298383, -1, 3610219, 923, 10000, 2007, 1, "")
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100132)    --���·�����Ѯ������20:30-21:30������С˵�����������Ʒɽ�������ڷ���ġ�923,2007��������������������Ի�ø������һ�������ֿ⣡��������ÿ��������һ�Σ�ÿ���ۼ����������3�Σ�
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
		end

		if (curhour == 20 and curmin == 45) or (curhour == 21 and curmin == 15) then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100132)    --���·�����Ѯ������20:30-21:30������С˵�����������Ʒɽ�������ڷ���ġ�923,2007��������������������Ի�ø������һ�������ֿ⣡��������ÿ��������һ�Σ�ÿ���ۼ����������3�Σ�
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
		elseif curhour == 21 and curmin == 30 then
		    map.MapDeleteCreature(3017298383, -1, shenyunfei_ID)
			shenyunfei_ID = 0
		end
	end

	-- ������ڼ䣬ÿ�յ�20:00��տ�����ľ������

	if Zhongqiujie == 1	then

		if (curmon == 9 and curday == 22)	and  (curhour== 19 and curmin==55)  then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 100, 3901)	--�����ٹ�5���Ӿ�Ҫ��ʼ��,���λ��ҵ���ˮ�ҵ����϶�[1000,969]�����[955��970]������[976��969]�����ǹ��ȼѽڣ�
		msg.DispatchWorldMsgEvent(MsgID)
		end

		if 	curhour == 20 and curmin == 0 then
			zhongqiu_kanfahenji = 0
		end
		if 	curhour >=19 and curhour <=21 then
			yueguangbaohe  = 1
		else
			yueguangbaohe  = 0
		end

	end
	if Guoqingjie == 1 then
		if (curmon == 9 and curday == 28) and (curhour == 17 and curmin == 55) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 4001)	--�ٹ�5���ӣ�������Ҫ��ʼ�ˣ�ף��ҹ������죬����ӭ���ӻԾ���룻
			msg.DispatchWorldMsgEvent(MsgID)
		end
		if curhour == 10 and  curmin == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 4027)	--��������ı���ʼ�ˣ����ҵ� ��ˮ��1677,701���ι۹����ı�
			msg.DispatchWorldMsgEvent(MsgID)
		end
		if curhour == 19 and curmin == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 100, 4028)	--���������ı���ʼ�ˣ����ҵ� ��¹��2106,2677���ι��ı�
			msg.DispatchWorldMsgEvent(MsgID)
		end

	end

	--[[�Ͷ���װ����ȡ¯����������¯ ˢ��ʱ��
	if act.GetActIsStart(134) ~= 1 and (curweekday == 2 or curweekday == 4 or curweekday == 6 or curweekday == 0)then
		if curhour >= 18 and curhour <= 22  and Labor_Creature[1].CreID == 0 then
			for k = 1,2 do
				Labor_Creature[k].CreID = map.MapCreateColCreature(3017299663, -1, Labor_Creature[k].TypeID, Labor_Creature[k].x, Labor_Creature[k].y, Labor_Creature[k].z, 1)
			end
		elseif Labor_Creature[1].CreID ~= 0 and curhour == 23 then
			for k = 1,2 do
				map.MapDeleteCreature(3017299663, -1, Labor_Creature[k].CreID)
				Labor_Creature[k].CreID = 0
			end
		end
	end]]
end

--ע��û������¼�
aux.RegisterActEvent(99, 4, "SystemActivity_OnTimerMin")

-- ��ս��ʱ����
function CityWar_OnTimerSec(Sec, actID)

	if CityStruggle_IsStart == 0 then
		return;
	end

	-- ÿ��3����л����Χ������buffer2029001
	-- ��������������Ӫ���
	if Sec % 25 == 0 then
		if Role_Alliance ~= nil then
			for k, v in pairs(Role_Alliance) do
				--  0--��������
				if v == 0 then
					if Atk_ZhaoHunFan.use == 1 then
						-- ������Һ��л�����꣬������룬���������750������Ҽ�buffer
						local bAddBuffer = aux.CalUnitDist(city_id[actID].MapID, -1, k, Atk_ZhaoHunFan.CreatureID)
						if bAddBuffer ~= nil and bAddBuffer == 1 then
							unit.AddBuff(city_id[actID].MapID, -1, k, 2029001, k)
						end
					end
				-- 1--��������
				elseif v == 1 then
					if Def_ZhaoHunFan.use == 1 then
						-- ������Һ��л�����꣬������룬���������750������Ҽ�buffer
						local bAddBuffer = aux.CalUnitDist(city_id[actID].MapID, -1, k, Def_ZhaoHunFan.CreatureID)
						if bAddBuffer ~= nil and bAddBuffer == 1 then
							unit.AddBuff(city_id[actID].MapID, -1, k, 2029001, k)
						end
					end
				end
			end
		end
	end
end

function Dragon_UseSkill()
	if dragon_timerbegin == 1 then
	    if dragon_timer % 5 == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 3803)    --�綾Ģ���Ѿ����ٻ�������X���Ģ����Ը����ĵ�λ��ɾ޴��˺���
			msg.AddMsgEvent(MsgID, 9, dragon_timer/5)
			msg.DispatchMapMsgEvent(MsgID, 3017299919, -1)
		end
		dragon_timer = dragon_timer - 1

		if dragon_timer < 1 then
		    dragon_timerbegin = 0
		end
	end
end


function SystemActivity_OnTimerSec(ActiveID, Sec)
       Dragon_UseSkill()
	-- ��ս��ʱ����
	CityWar_OnTimerSec(Sec, 70) -- �������


--[[    --ÿ5�����Ӣ�������Ⱥ����¹�ĵ�ͼ�е���ҽ���һ�ο�������
    if act.GetActIsStart(14) or act.GetActIsStart(97) then
	    local Sec   = tonumber(os.date("%S"))
        if Sec % 60 == 0 then
		    local RoleTbl1 = map.GetMapAllRoleID(2983744463, 1)
			local RoleTbl2 = map.GetMapAllRoleID(2983744463, 2)
			local RoleTbl3 = map.GetMapAllRoleID(2983744463, 3)
			local RoleTbl4 = map.GetMapAllRoleID(2983744463, 4)
			if RoleTbl1 then
				for k,v in pairs(RoleTbl1) do
					--�������ΪPK״̬(���ڿͻ��˵����жϣ�
                    if role.IsRoleInStatus(v,4) ~= nil and role.IsRoleInStatus(v,4) ~= true then
					    role.SetRoleState(v, 4, true)
					end
				end
			end
			if RoleTbl2 then
				for k,v in pairs(RoleTbl2) do
					--�������ΪPK״̬(���ڿͻ��˵����жϣ�
					if role.IsRoleInStatus(v,4) ~= nil and role.IsRoleInStatus(v,4) ~= true then
					    role.SetRoleState(v, 4, true)
					end
				end
			end
			if RoleTbl3 then
				for k,v in pairs(RoleTbl3) do
					--�������ΪPK״̬(���ڿͻ��˵����жϣ�
					if role.IsRoleInStatus(v,4) ~= nil and role.IsRoleInStatus(v,4) ~= true then
					    role.SetRoleState(v, 4, true)
					end
				end
			end
			if RoleTbl4 then
				for k,v in pairs(RoleTbl4) do
					--�������ΪPK״̬(���ڿͻ��˵����жϣ�
					if role.IsRoleInStatus(v,4) ~= nil and role.IsRoleInStatus(v,4) ~= true then
					    role.SetRoleState(v, 4, false)
					end
				end
			end
		end
    elseif act.GetActIsStart(18) or act.GetActIsStart(103) then
	    local Sec   = tonumber(os.date("%S"))
        if Sec % 60 == 0 then
		    local RoleTbl1 = map.GetMapAllRoleID(2983743695, 1)
			local RoleTbl2 = map.GetMapAllRoleID(2983743695, 2)
			local RoleTbl3 = map.GetMapAllRoleID(2983743695, 3)
			local RoleTbl4 = map.GetMapAllRoleID(2983743695, 4)
			if RoleTbl1 then
				for k,v in pairs(RoleTbl1) do
					--�������ΪPK״̬(���ڿͻ��˵����жϣ�
					if role.IsRoleInStatus(v,4) ~= nil and role.IsRoleInStatus(v,4) ~= true then
					    role.SetRoleState(v, 4, true)
					end
				end
			end
			if RoleTbl2 then
				for k,v in pairs(RoleTbl2) do
					--�������ΪPK״̬(���ڿͻ��˵����жϣ�
					if role.IsRoleInStatus(v,4) ~= nil and role.IsRoleInStatus(v,4) ~= true then
					    role.SetRoleState(v, 4, true)
					end
				end
		    end
			if RoleTbl3 then
				for k,v in pairs(RoleTbl3) do
					--�������ΪPK״̬(���ڿͻ��˵����жϣ�
					if role.IsRoleInStatus(v,4) ~= nil and role.IsRoleInStatus(v,4) ~= true then
					    role.SetRoleState(v, 4, true)
					end
				end
			end
			if RoleTbl4 then
				for k,v in pairs(RoleTbl4) do
					--�������ΪPK״̬(���ڿͻ��˵����жϣ�
					if role.IsRoleInStatus(v,4) ~= nil and role.IsRoleInStatus(v,4) ~= true then
					    role.SetRoleState(v, 4, true)
					end
				end
			end
		end
	end]]
end
--ע��û������¼�
aux.RegisterActEvent(99, 1, "SystemActivity_OnTimerSec")

function KuiLingBaoShu_OnClick(MapID, InstanceID, RoleID, TOMsgID, ToMapID, x, y, z)
    if TOMsgID == 3176 then
		if role.IsRoleInStatus(RoleID, 4096) then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 3177) -- ������������֮�У��޷�ʹ�øô��͹���
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
		else
			if KuiLingBaoShu_min == 1 or KuiLingBaoShu_min == 2 then
				if role.IsRoleInStatus(RoleID, 1024) then
					role.CancelRoleStallState(MapID, InstanceID, RoleID)
				end
				role.RoleGotoNewMap(MapID, InstanceID, RoleID, ToMapID, x, y, z)      --����
			else
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 2779) -- �����ȷ����ʱ������Ѿ�������1���ӵ�ʱ�����ƣ�������ǰ���μӻ��
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --������Ϣ
			end
		end
	end
end

--ע��û������¼�
aux.RegisterActEvent(99, 8, "KuiLingBaoShu_OnClick")

